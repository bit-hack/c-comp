#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

#define STACKLEN 1024

#define FRAMESIZE 2         // old FP, PC

int cCode[NCODELEN];        // output code stream
int cCodeLen;               // output code written

int vStack[STACKLEN];       // evaluation stack
int vStackPtr;              // stack pointer

int vPC;                    // program counter 
int vFP;                    // frame pointer

void fatal(char *msg, ...) {
  va_list args;
  va_start(args, msg);
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(1);
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
  case TOK_ADD: res = lhs + rhs; break;
  case TOK_SUB: res = lhs - rhs; break;
  }
  vPush(res);
}

void vAssign() {
  int rvalue = vPop();
  int lvalue = vPop();
  if (lvalue < 0 || lvalue >= vStackPtr) {
    fatal("error: invalid lvalue address");
  }
  vStack[lvalue] = rvalue;
}

void vInsCall(int opr) {
  // save old stack frame
  vPush(vFP);
  // save return address (next inst)
  vPush(vPC);
  // jump to function
  vPC = opr;
}

void vInsAlloc(int opr) {
  vStackPtr += opr;
  if (vStackPtr >= STACKLEN) {
    fatal("error: stack overflow");
  }
}

void vInsScall(int opr) {
  if (opr == 0) {
    // putchar()
  }
}

void vReturn(int opr) {
  // get return value
  int ret = vPop();
  // discard locals
  vStackPtr = vFP;
  // jump to return address
  vPC = vPop();
  // save old stack frame
  vFP = vPop();
  // remove arguments
  vStackPtr -= opr;
  // place return value on stack
  vPush(ret);

  if (vFP == 0) {
    fatal("return from main");
  }
}

void vStep() {

  int ins = cCode[ vPC++ ];

  // zero operand instructions
  switch (ins) {
  case INS_DEREF:   vInsDeref();  return;
  case INS_DROP:    vPop();       return;
  case TOK_ASSIGN:  vAssign();    return;
  case TOK_OR:      vInsAlu(ins); return;
  case TOK_AND:     vInsAlu(ins); return;
  case TOK_ADD:     vInsAlu(ins); return;
  case TOK_SUB:     vInsAlu(ins); return;
  case TOK_MUL:     vInsAlu(ins); return;
  case TOK_DIV:     vInsAlu(ins); return;
  case TOK_EQU:     vInsAlu(ins); return;
  case TOK_LOGOR:   vInsAlu(ins); return;
  case TOK_LOGAND:  vInsAlu(ins); return;
  case TOK_BITOR:   vInsAlu(ins); return;
  case TOK_MOD:     vInsAlu(ins); return;
  case TOK_LT:      vInsAlu(ins); return;
  case TOK_GT:      vInsAlu(ins); return;
  case TOK_LTEQU:   vInsAlu(ins); return;
  case TOK_GTEQU:   vInsAlu(ins); return;
  case TOK_NEQU:    vInsAlu(ins); return;
  case TOK_LOGNOT:  vInsAlu(ins); return;
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
  case INS_RETURN:  vReturn(opr);                   return;
  case INS_JMP:                      vPC = opr;     return;
  case INS_JZ:      if (vPop() == 0) vPC = opr;     return;
  case INS_JNZ:     if (vPop() != 0) vPC = opr;     return;
  case INS_SCALL:   vInsScall(opr);                 return;
  }

  fatal("error: unknown instruction");
}

int main() {

  // execution loop
  int i=1000;
  while (i--) {
    vStep();
  }

  return 0;
}
