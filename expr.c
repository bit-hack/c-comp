#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TOK_EOF     0   // \0
#define TOK_ASSIGN  1   // =
#define TOK_OR      2   // |
#define TOK_AND     3   // &
#define TOK_ADD     4   // +
#define TOK_SUB     5   // -
#define TOK_MUL     6   // *
#define TOK_DIV     7   // /
#define TOK_INC     8   // ++
#define TOK_DEC     9   // --
#define TOK_EQUALS  10  // ==
#define TOK_LOGOR   11  // ||
#define TOK_BITOR   12  // |
#define TOK_LOGAND  13  // &&
#define TOK_BITAND  14  // &
#define TOK_SEMI    15  // ;
#define TOK_LPAREN  16  // (
#define TOK_RPAREN  17  // )
#define TOK_SYMBOL  128
#define TOK_LITERAL 129

#define token_t int

#define bool int
#define true  1
#define false 0

void pExpr(int v);

char lK0, lK1;      // input delay line (lK1 = lookahead)

char lSymbol[128];  // currently parsed symbol
int  lSymLen;       // current symbol length

token_t tToken;     // last parsed token
bool    tPeeked;    // a peek was performed

void fatal(char *msg) {
  fputs(msg, stderr);
  fputs("\n", stderr);
  exit(1);
}

bool lIsWhiteSpace(char c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool lIsNumber(char c) {
  return c >= '0' && c <= '9';
}

bool lIsAlpha(char c) {
  return c >= 'A' && c <= 'Z' ||
         c >= 'a' && c <= 'z' ||
         c == '_';
}

// get next character from input stream
char lNext() {
  int in = fgetc(stdin);
  lK0 = lK1;
  lK1 = (in >= 0) ? in : '\0';
  return lK0;
}

// return true if lookahead is a specific char
bool lFound(char c) {
  if (lK1 == c) {
    lNext();
    return true;
  }
  return false;
}

// peek next character in input stream
char lPeek() {
  return lK1;
}

// next token
int tNext() {

  // if peeked, just mark as consumed and return
  if (tPeeked) {
    tPeeked = false;
    return tToken;
  }

  char c = lNext();

  // consume whitespace
  while (lIsWhiteSpace(c)) {
    c = lNext();
  }

  lSymLen = 0;

  // tokenize literal values
  if (lIsNumber(c)) {
    for (;;c = lNext()) {
      lSymbol[lSymLen++] = c;
      if (!lIsNumber(lK1))
        break;
    }
    lSymbol[lSymLen] = '\0';
    return tToken = TOK_LITERAL;
  }

  // tokenize alphanumeric symbols
  if (lIsAlpha(c)) {
    for (;;c = lNext()) {
      lSymbol[lSymLen++] = c;
      if (!lIsAlpha(lK1) && !lIsNumber(lK1))
        break;
    }
    lSymbol[lSymLen] = '\0';
    return tToken = TOK_SYMBOL;
  }

  // try simple tokens
  switch (c) {
  case '\0': return tToken = TOK_EOF;
  case '/':  return tToken = TOK_DIV;
  case '*':  return tToken = TOK_MUL;
  case ';':  return tToken = TOK_SEMI;
  case '(':  return tToken = TOK_LPAREN;
  case ')':  return tToken = TOK_RPAREN;
  case '|':  return tToken = lFound('|') ? TOK_LOGOR  : TOK_BITOR;
  case '&':  return tToken = lFound('&') ? TOK_LOGAND : TOK_BITAND;
  case '+':  return tToken = lFound('+') ? TOK_INC    : TOK_ADD;
  case '-':  return tToken = lFound('-') ? TOK_DEC    : TOK_SUB;
  case '=':  return tToken = lFound('=') ? TOK_EQUALS : TOK_ASSIGN;
  }

  // unknown token in input stream
  printf("unknown tok %u\n", c);
  fatal("Unknown token in input stream");
  return tToken = TOK_EOF;
}

// advance to next token but dont treat as consumed
// warning, a peek will wipe out the previous token.
token_t tPeek() {
  tNext();
  tPeeked = true;
  return tToken;
}

// print a token
void tPrint(int t) {
  switch (t) {
  case TOK_SYMBOL:
  case TOK_LITERAL: puts(lSymbol); break;
  case TOK_EOF    : puts("\0");    break;
  case TOK_ASSIGN : puts("=");     break;
  case TOK_OR     : puts("|");     break;
  case TOK_AND    : puts("&");     break;
  case TOK_ADD    : puts("+");     break;
  case TOK_SUB    : puts("-");     break;
  case TOK_MUL    : puts("*");     break;
  case TOK_DIV    : puts("/");     break;
  case TOK_INC    : puts("++");    break;
  case TOK_DEC    : puts("--");    break;
  case TOK_EQUALS : puts("==");    break;
  case TOK_LOGOR  : puts("||");    break;
  case TOK_BITOR  : puts("|");     break;
  case TOK_LOGAND : puts("&&");    break;
  case TOK_BITAND : puts("&");     break;
  case TOK_SEMI   : puts(";");     break;
  case TOK_LPAREN : puts("(");     break;
  case TOK_RPAREN : puts(")");     break;
  default:
    fatal("unknown token");
  }
}

// expect a specific token
void tExpect(token_t tok) {
  token_t got = tNext();
  if (got != tok) {
    printf("expected %u got %u\n", tok, got);
    fatal("unexpected token");
  }
}

// return true if a specific token was found
bool tFound(token_t tok) {
  if (tPeek() == tok) {
    lNext();  // consume
    return true;
  }
  return false;
}

// emit to output stream
void cEmit(int c) {
//  fputc(c, stdout);
}

// consume a primary expression
void pPrimary() {
  token_t n = tNext();

  if (n == TOK_LPAREN) {
    pExpr(1);
    tExpect(TOK_RPAREN);
    return;
  }

  if (n == TOK_LITERAL) {
    tPrint(n);
    cEmit(n);
    return;
  }

  if (n == TOK_SYMBOL) {
    tPrint(n);
    cEmit(n);

    // xxx if ( function call
    // xxx if [ array access

    return;
  }

  fatal("expected literal or name");
}

// the precedence table
int pPrec(token_t c) {
  switch (c) {
  case TOK_ASSIGN: return 1;
  case TOK_BITAND: return 2;
  case TOK_BITOR:  return 2;
  case TOK_ADD:    return 3;
  case TOK_SUB:    return 3;
  case TOK_MUL:    return 4;
  case TOK_DIV:    return 4;
  }
  return 0;
}

// return true if this token is an operator
bool pIsOperator(token_t c) {
  return pPrec(c) > 0;
}

// precedence climbing expression parser
void pExpr(int min_prec) {
  // lhs
  pPrimary();
  // while our operator is equal or higher precidence
  while (1) {
    // look ahead for possible operators
    token_t op = tPeek();
    if (!pIsOperator(op)) {
        return;
    }
    if (pPrec(op) < min_prec) {
        return;
    }
    // consume operator
    tNext();

    // rhs
    pExpr(pPrec(op));

    tPrint(op);
    cEmit(op);
  }
}

int main() {

  // discard first read (c0 invalid)
  lNext();

  while (lPeek() != TOK_EOF) {
    // parse an expression
    pExpr(1);
  }

  return 0;
}
