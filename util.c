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
bool strMatch(char *a, char *b) {
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
char *strSkip(char *c) {
  while (*c++);
  return c;
}

// copy a string from src to dst
char *strCopy(char *dst, char *src) {
  while (*src) {
    *dst++ = *src++;
  }
  *dst++ = '\0';
  return dst;
}

// convert a string to an integer
int strToInt(char *a) {
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

char *tokName(token_t tok) {
  switch (tok) {
  case TOK_EOF:     return "\\0";
  case TOK_ASSIGN:  return "=";
  case TOK_ADD:     return "+";
  case TOK_SUB:     return "-";
  case TOK_MUL:     return "*";
  case TOK_DIV:     return "/";
  case TOK_INC:     return "++";
  case TOK_DEC:     return "--";
  case TOK_LOGOR:   return "||";
  case TOK_BITOR:   return "|";
  case TOK_LOGAND:  return "&&";
  case TOK_BITAND:  return "&";
  case TOK_SEMI:    return ";";
  case TOK_LPAREN:  return "(";
  case TOK_RPAREN:  return ")";
  case TOK_COMMA:   return ",";
  case TOK_LBRACE:  return "{";
  case TOK_RBRACE:  return "}";
  case TOK_MOD:     return "%";
  case TOK_LT:      return "<";
  case TOK_GT:      return ">";
  case TOK_LTEQU:   return "<=";
  case TOK_GTEQU:   return ">=";
  case TOK_EQU:     return "==";
  case TOK_NEQU:    return "!=";
  case TOK_LOGNOT:  return "!";
  case TOK_BITNOT:  return "~";
  case TOK_LBRACK:  return "[";
  case TOK_RBRACK:  return "]";
  case TOK_SYMBOL:  return "symbol";
  case TOK_LITERAL: return "literal";
  case TOK_IF:      return "if";
  case TOK_INT:     return "int";
  case TOK_RETURN:  return "return";
  case TOK_ELSE:    return "else";
  case TOK_CHAR:    return "char";
  case TOK_VOID:    return "void";
  case TOK_WHILE:   return "while";
  case TOK_DO:      return "do";
  default:          return "unknown";
  }
}
