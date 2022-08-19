#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

#define STACKLEN 1024

#define FRAMESIZE 2         // old FP, PC

int cCode[NCODELEN];        // code stream
int cCodeLen;               // code length

int vStack[STACKLEN];       // evaluation stack
int vStackPtr;              // stack pointer

int vPC;                    // program counter 
int vFP;                    // frame pointer

int vPeek() {
  return vStack[vStackPtr - 1];
}

void vPush(int v) {
  if (vStackPtr >= STACKLEN) {
    fatal("error: stack overflow");
  }
  vStack[vStackPtr++] = v;
}

int vPop() {
  if (vStackPtr <= 0) {
    fatal("error: stack underflow");
  }
  return vStack[--vStackPtr];
}

void vInsDeref() {
  int ptr = vPop();
  if (ptr < 0 || ptr >= vStackPtr) {
    fatal("error: invalid dereference");
  }
  vPush(vStack[ptr]);
}

void vInsAlu(int ins) {
  int rhs = vPop();
  int lhs = vPop();
  int res = 0;

  switch (ins) {
  case TOK_MOD:
  case TOK_DIV:
    if (rhs == 0) {
      fatal("error: division by zero");
    }
  }

  switch (ins) {
  case TOK_ADD:     res = lhs +  rhs; break;
  case TOK_SUB:     res = lhs -  rhs; break;
  case TOK_MUL:     res = lhs *  rhs; break;
  case TOK_EQU:     res = lhs == rhs; break;
  case TOK_NEQU:    res = lhs != rhs; break;
  case TOK_LOGOR:   res = lhs || rhs; break;
  case TOK_LOGAND:  res = lhs && rhs; break;
  case TOK_BITOR:   res = lhs |  rhs; break;
  case TOK_BITAND:  res = lhs &  rhs; break;

  case TOK_DIV:     res = lhs /  rhs; break;
  case TOK_MOD:     res = lhs %  rhs; break;

  case TOK_LT:      res = lhs <  rhs; break;
  case TOK_GT:      res = lhs >  rhs; break;
  case TOK_LTEQU:   res = lhs <= rhs; break;
  case TOK_GTEQU:   res = lhs >= rhs; break;
  }
  vPush(res);
}

void vInsAssign() {
  int rvalue = vPop();
  int lvalue = vPop();
  if (lvalue < 0 || lvalue >= vStackPtr) {
    fatal("error: invalid lvalue address");
  }
  vPush(vStack[lvalue] = rvalue);
}

void vInsCall(int opr) {
  // save old stack frame
  vPush(vFP);
  // save return address (next inst)
  vPush(vPC);
  // jump to function
  vPC = opr;
  // start new stack frame
  vFP = vStackPtr;
}

void vInsReturn(int opr) {
  // pop return value
  int ret = vPop();
  // remove all locals
  vStackPtr = vFP;
  // pop return address
  vPC = vPop();
  // restore old stack frame
  vFP = vPop();
  // remove arguments
  vStackPtr -= opr;
  // place return value back on stack
  vPush(ret);

  if (vFP == 0) {
    int ret = vPop();
    printf("return from main (%d)\n", ret);
    exit(0);
  }
}

void vInsAlloc(int opr) {  
  if (vStackPtr >= STACKLEN) {
    fatal("error: stack overflow");
  }
  // zero stack and enlarge
  for (int i=0; i<opr; ++i) {
    vStack[vStackPtr++] = 0;
  }
}

void vInsScall(int opr) {
  if (opr == /*putchar*/0) {
    putchar(vPop());
    vPush(0);
    return;
  }
  fatal("error: unknown systemcall");
}

void vStep() {

  int ins = cCode[ vPC++ ];

  // zero operand instructions
  switch (ins) {
  case INS_DEREF:   vInsDeref();    return;
  case INS_DROP:    vPop();         return;
  case TOK_ASSIGN:  vInsAssign();   return;
  case TOK_ADD:     vInsAlu(ins);   return;
  case TOK_SUB:     vInsAlu(ins);   return;
  case TOK_MUL:     vInsAlu(ins);   return;
  case TOK_DIV:     vInsAlu(ins);   return;
  case TOK_LOGOR:   vInsAlu(ins);   return;
  case TOK_LOGAND:  vInsAlu(ins);   return;
  case TOK_BITOR:   vInsAlu(ins);   return;
  case TOK_MOD:     vInsAlu(ins);   return;
  case TOK_LT:      vInsAlu(ins);   return;
  case TOK_GT:      vInsAlu(ins);   return;
  case TOK_LTEQU:   vInsAlu(ins);   return;
  case TOK_GTEQU:   vInsAlu(ins);   return;
  case TOK_EQU:     vInsAlu(ins);   return;
  case TOK_NEQU:    vInsAlu(ins);   return;
  case INS_NEG:     vPush(-vPop()); return;
  }

  int opr = cCode[ vPC++ ];

  // one operand instructions
  switch (ins) {
  case INS_CONST:   vPush(opr);                     return;
  case INS_CALL:    vInsCall(opr);                  return;
  case INS_GETAG:   vPush(opr);                     return;
  case INS_GETAL:   vPush(vFP + opr);               return;
  case INS_GETAA:   vPush(vFP - opr - FRAMESIZE);   return;
  case INS_ALLOC:   vInsAlloc(opr);                 return;
  case INS_RETURN:  vInsReturn(opr);                return;
  case INS_JMP:                      vPC = opr;     return;
  case INS_JZ:      if (vPop() == 0) vPC = opr;     return;
  case INS_JNZ:     if (vPop() != 0) vPC = opr;     return;
  case INS_SCALL:   vInsScall(opr);                 return;
  }

  fatal("error: unknown instruction");
}

int main(int argc, char **args) {

  cCodeLen = fread(cCode, 4, NCODELEN, stdin);
  if (ferror(stdin)) {
    fatal("stdin error");
  }

  // execution loop
  int i=4000;
  while (i--) {
    if (argc > 1) {
      printf("TOS=%-3u  | ", vPeek());
      dasm(cCode + vPC, vPC);
      printf("\n");
    }
    vStep();
  }

  if (i < 0) {
    fatal("error: program did not complete");
  }

  return 0;
}
