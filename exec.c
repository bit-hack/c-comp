#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

#define NMEMORY 1024*1024

#define FRAMESIZE 2         // old FP, PC

int cCode[NMEMORY];         // code stream
int cCodeLen;               // code length

int *vStack = cCode;
int vStackBase;             // bottom of stack
int vStackPtr;              // stack pointer

int vPC;                    // program counter
int vFP;                    // frame pointer
int vST;                    // string table

int vPeek(int b) {
  return vStack[vStackPtr - (1 + b)];
}

void vPush(int v) {
  if (vStackPtr >= NMEMORY) {
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

  if (vFP <= vStackBase) {
    int ret = vPop();
    exit(ret);
  }
}

void vInsAlloc(int opr) {
  if (vStackPtr >= NMEMORY) {
    fatal("error: stack overflow");
  }
  // zero stack and enlarge
  for (int i=0; i<opr; ++i) {
    vStack[vStackPtr++] = 0;
  }
}

void vPrintInt(const char *fmt, int value) {
  printf(fmt, value);
}

void vPrintStr(int addr) {
  int c;
  while (c = vStack[addr++]) {
    putchar(c);
  }
}

void vSysPutchar(int opr, int nargs) {
  (void)nargs;
  putchar(vPop());
  vPush(0);  // return value
}

void vSysPuts(int opr, int nargs) {
  (void)nargs;
  int addr = vPop();
  vPrintStr(addr);
  vPush(0);  // return value
}

void vSysPrintf(int opr, int n) {

  if (n == 0) {
    fatal("error: insufficient arguments to printf");
  }
  char c;
  int nargs = n;
  int fmt = vPeek(--n);

  bool mod = false;
  while (c = vStack[fmt++]) {
    if (mod) {
      if (n <= 0) {
        fatal("error: insufficient arguments to printf");
      }
      int val = vPeek(--n);
      switch (c) {
      case 'd': vPrintInt("%d", val); break;
      case 'u': vPrintInt("%u", val); break;
      case 'c': vPrintInt("%c", val); break;
      case 's': vPrintInt("%s", val); break;
      }
      mod = false;
    } else {
      if (c == '%') {
        mod = true;
      }
      else {
        putchar(c);
      }
    }
  }
  // pop all arguments
  while (nargs--) {
    vPop();
  }
  vPush(0);
}

void vSysGetchar() {
  int r = getchar();
  vPush(r);
}

void vSysExit() {
  int code = vPop();
  exit(code);
  vPush(0);
}

void vInsScall(int opr) {

  int nargs = vPop();

  switch (opr) {
  case 0: vSysPutchar(opr, nargs); break;
  case 1: vSysPuts   (opr, nargs); break;
  case 2: vSysPrintf (opr, nargs); break;
  case 3: vSysGetchar(opr, nargs); break;
  case 4: vSysExit   (opr, nargs); break;
  default: fatal("error: unknown systemcall");
  }
}

void vInsSwap() {
  int x = vPop();
  int y = vPop();
  vPush(x);
  vPush(y);
}

void vStep() {

  if (vPC < 0 || vPC >= cCodeLen) {
    fatal("error: invalid vPC 0x%08x", vPC);
  }

  int ins = cCode[ vPC++ ];

  // zero operand instructions
  switch (ins) {
  case INS_DEREF:   vInsDeref();     return;
  case INS_DROP:    vPop();          return;
  case TOK_LOGNOT:  vPush(!vPop());  return;
  case TOK_ASSIGN:  vInsAssign();    return;
  case TOK_ADD:     vInsAlu(ins);    return;
  case TOK_SUB:     vInsAlu(ins);    return;
  case TOK_MUL:     vInsAlu(ins);    return;
  case TOK_DIV:     vInsAlu(ins);    return;
  case TOK_LOGOR:   vInsAlu(ins);    return;
  case TOK_BITOR:   vInsAlu(ins);    return;
  case TOK_LOGAND:  vInsAlu(ins);    return;
  case TOK_BITAND:  vInsAlu(ins);    return;
  case TOK_MOD:     vInsAlu(ins);    return;
  case TOK_LT:      vInsAlu(ins);    return;
  case TOK_GT:      vInsAlu(ins);    return;
  case TOK_LTEQU:   vInsAlu(ins);    return;
  case TOK_GTEQU:   vInsAlu(ins);    return;
  case TOK_EQU:     vInsAlu(ins);    return;
  case TOK_NEQU:    vInsAlu(ins);    return;
  case INS_NEG:     vPush(-vPop());  return;
  case INS_DUP:     vPush(vPeek(0)); return;
  case INS_SWAP:    vInsSwap();      return;
  }

  int opr = cCode[ vPC++ ];

  // one operand instructions
  switch (ins) {
  case INS_STRTAB:  vST = opr;                      return;
  case INS_STR:     vPush(vST + opr);               return;
  case INS_CONST:   vPush(opr);                     return;
  case INS_CALL:    vInsCall(opr);                  return;
  case INS_GETAG:   vPush(vStackBase + opr);        return;
  case INS_GETAL:   vPush(vFP + opr);               return;
  case INS_GETAA:   vPush(vFP - opr - FRAMESIZE);   return;
  case INS_ALLOC:   vInsAlloc(opr);                 return;
  case INS_RETURN:  vInsReturn(opr);                return;
  case INS_JMP:                      vPC = opr;     return;
  case INS_JZ:      if (vPop() == 0) vPC = opr;     return;
  case INS_JNZ:     if (vPop() != 0) vPC = opr;     return;
  case INS_SCALL:   vInsScall(opr);                 return;
  }

  fatal("error: unknown instruction %u", ins);
}

int main(int argc, char **args) {

  FILE *fd = stdin;
  if (argc > 1) {
    fd = fopen(args[1], "r");
  }
  if (!fd) {
    fatal("error: unable to open input file");
  }

  int trace = (argc > 2);

  cCodeLen = fread(cCode, 4, NCODELEN, fd);
  if (ferror(stdin)) {
    fatal("error: fread error");
  }

  // start the stack after the code
  vStackBase = cCodeLen;
  vStackPtr  = cCodeLen;

  // execution loop
  int max_insts=-1;
  while (--max_insts) {
    if (trace) {
      printf("%4d, %4d,  | ", vPeek(0), vPeek(1));
      dasm(cCode + vPC, vPC);
      printf("\n");
    }
    vStep();
  }

  if (max_insts <= 0) {
    fatal("error: program did not complete");
  }

  return 0;
}
