#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

void fatal(char *msg, ...) {
  va_list args;
  va_start(args, msg);
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
  va_end(args);
  exit(1);
}

// check if two strings match
bool strmatch(char *a, char *b) {
  while (true) {
    if (*a == '\0' && *b == '\0') {
      return true;
    }
    if (*a++ != *b++) {
      return false;
    }
  }
}

// skip a string
char *strskip(char *c) {
  while (*c++);
  return c;
}

// copy a string from src to dst
char *strcopy(char *dst, char *src) {
  while (*src) {
    *dst++ = *src++;
  }
  *dst++ = '\0';
  return dst;
}

// convert a string to an integer
int strint(char *a) {
  int val = 0;
  while (*a != '\0') {
    val = val * 10 + *a++ - '0';
  }
  return val;
}

int contains(symbol_t find, symbol_t *arr, int count) {
  int i;
  for (i=0; i<count; ++i) {
    if (arr[i] == find) {
      // position it was found in
      return i;
    }
  }
  // not found
  return -1;
}

#define DASM0(INS, NAME) \
  case INS: printf("%2u  %-6s\n", loc, NAME); return 1;

#define DASM1(INS, NAME) \
  case INS: printf("%2u  %-6s %u\n", loc, NAME, opr); return 2;

int dasm(int *cCode, int loc) {
  int ins = cCode[0];
  int opr = cCode[1];
  switch (ins) {
  DASM0(INS_DEREF,  "DEREF");
  DASM1(INS_CALL,   "CALL");
  DASM1(INS_CONST,  "CONST");
  DASM1(INS_GETAG,  "GETAG");
  DASM1(INS_GETAL,  "GETAL");
  DASM1(INS_GETAA,  "GETAA");
  DASM1(INS_ALLOC,  "ALLOC");
  DASM0(TOK_ASSIGN, "ASSIGN");
  DASM0(TOK_ADD,    "ADD");
  DASM0(TOK_SUB,    "SUB");
  DASM0(TOK_MUL,    "MUL");
  DASM0(TOK_DIV,    "DIV");
  DASM0(TOK_EQU,    "EQU");
  DASM0(TOK_NEQU,   "NEQU");
  DASM0(TOK_LOGOR,  "LOGOR");
  DASM0(TOK_LOGAND, "LOGAND");
  DASM0(TOK_BITOR,  "BITOR");
  DASM0(TOK_MOD,    "MOD");
  DASM0(TOK_LT,     "LT");
  DASM0(TOK_GT,     "GT");
  DASM0(TOK_LTEQU,  "LTEQU");
  DASM0(TOK_GTEQU,  "GTEQU");
  DASM1(INS_RETURN, "RETURN");
  DASM1(INS_JMP,    "JMP");
  DASM1(INS_JZ,     "JZ");
  DASM1(INS_JNZ,    "JNZ");
  DASM0(INS_DROP,   "DROP");
  DASM1(INS_SCALL,  "SCALL");
  DASM0(INS_NEG,    "NEG");
  default:
    fatal("Unknown instruction %u at %u", ins, loc);
    return 0;
  }
}
