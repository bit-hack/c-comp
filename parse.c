#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "defs.h"

char     lK0, lK1;              // input delay line (lK1 = lookahead)
int      lLine;                 // currently lexed line number

char     tSym[128];             // extracted currently parsed token
int      tSymLen;               // current symbol length
int      tSymValue;             // value if symbol is a literal

token_t  tToken;                // last parsed token
bool     tPeeked;               // a peek was performed

char     sSymTab[SYMTABLEN];    // symbol table
int      sSymLen;               // current length of symbol table

symbol_t sFuncTable[NFUNC];     // function table
int      sFuncPos  [NFUNC];     // function code offsets
int      sFuncCount;            // number of functions

symbol_t sGlobalTable[NGLOBAL]; // global table
int      sGlobalCount;          // number of globals

symbol_t sArgTable[NARG];       // argument table
int      sArgCount;             // number of arguments

symbol_t sLocalTable[NLOCAL];   // locals table
int      sLocalCount;           // number of locals

int      cCode[NCODELEN];       // output code stream
int      cCodeLen;              // output code written

FILE    *inFile;                // input file

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
  int in = fgetc(inFile);
  lK0 = lK1;
  lK1 = (in >= 0) ? in : '\0';
  return lK0;
}

// return true if lookahead is a specific char
bool lFound(char c) {
  return (lK1 == c) ? (lNext(), true) : false;
}

#define CHECK(X, T) if (strmatch(tSym, X)) return T;
token_t tKeywordCheck() {
  switch (tSym[0]) {
  case 'c': CHECK("char",   TOK_CHAR);   break;
  case 'd': CHECK("do",     TOK_DO);     break;
  case 'e': CHECK("else",   TOK_ELSE);   break;
  case 'i': CHECK("int",    TOK_INT);
            CHECK("if",     TOK_IF);     break;
  case 'v': CHECK("void",   TOK_VOID);   break;
  case 'r': CHECK("return", TOK_RETURN); break;
  case 'w': CHECK("while",  TOK_WHILE);  break;
  }
  return TOK_SYMBOL;
}
#undef CHECK

// return the next token
token_t tNext() {

  // if peeked, just mark as consumed and return
  if (tPeeked) {
    tPeeked = false;
    return tToken;
  }

  char c = lNext();

  // consume whitespace
  while (lIsWhiteSpace(c)) {
    lLine = (c == '\n') ? (lLine + 1) : lLine;
    c = lNext();
  }

  tSymLen = 0;

  // tokenize literal values
  if (lIsNumber(c)) {
    for (;;c = lNext()) {
      tSym[tSymLen++] = c;
      if (!lIsNumber(lK1))
        break;
    }
    tSym[tSymLen] = '\0';
    tSymValue = strint(tSym);
    return tToken = TOK_LITERAL;
  }

  // tokenize alphanumeric symbols
  if (lIsAlpha(c)) {
    for (;;c = lNext()) {
      tSym[tSymLen++] = c;
      if (!lIsAlpha(lK1) && !lIsNumber(lK1))
        break;
    }
    tSym[tSymLen] = '\0';
    return tToken = tKeywordCheck();
  }

  // tokenize character literals
  if (c == '\'') {
    char c = lNext();
    if (lNext() != '\'') {
      fatal("%u: error: malformed character literal", lLine);
    }
    tSymValue = c;
    return tToken = TOK_LITERAL;
  }

  // try simple tokens
  switch (c) {
  case '\0': return tToken = TOK_EOF;
  case '/':  return tToken = TOK_DIV;
  case '*':  return tToken = TOK_MUL;
  case ';':  return tToken = TOK_SEMI;
  case '(':  return tToken = TOK_LPAREN;
  case ')':  return tToken = TOK_RPAREN;
  case ',':  return tToken = TOK_COMMA;
  case '{':  return tToken = TOK_LBRACE;
  case '}':  return tToken = TOK_RBRACE;
  case '%':  return tToken = TOK_MOD;
  case '<':  return tToken = lFound('=') ? TOK_LTEQU  : TOK_LT;
  case '>':  return tToken = lFound('=') ? TOK_GTEQU  : TOK_GT;
  case '|':  return tToken = lFound('|') ? TOK_LOGOR  : TOK_BITOR;
  case '&':  return tToken = lFound('&') ? TOK_LOGAND : TOK_BITAND;
  case '+':  return tToken = lFound('+') ? TOK_INC    : TOK_ADD;
  case '-':  return tToken = lFound('-') ? TOK_DEC    : TOK_SUB;
  case '=':  return tToken = lFound('=') ? TOK_EQU    : TOK_ASSIGN;
  case '!':  return tToken = lFound('=') ? TOK_NEQU   : TOK_LOGNOT;
  }

  fatal("%u: error: unexpected character '%c'", lLine, c);
  return tToken = TOK_EOF;
}

// advance to next token but dont treat as consumed
// warning, a peek will wipe out the previous token
token_t tPeek() {
  tNext();
  tPeeked = true;
  return tToken;
}

// expect a specific token
void tExpect(token_t tok) {
  token_t got = tNext();
  if (got != tok) {
    fatal("%u: error: expecting token %u but got %u\n", lLine, tok, got);
  }
}

// return true and consume if a specific token was found
bool tFound(token_t tok) {
  return (tPeek() == tok) ? (tNext(), true) : false;
}

// intern the string held in 'tSym'
// add to symbol table if not present and return index
int sIntern(char *name) {
  // XXX: todo, check for symtab overflow
  char *c = sSymTab;
  int n = 0;
  for (;n < sSymLen; ++n) {
    if (strmatch(c, name)) {
      return n;
    }
    c = strskip(c);
  }
  strcopy(c, name);
  return sSymLen++;
}

// a new global is being declared
void sGlobalAdd(type_t type, symbol_t sym) {
  if (sGlobalCount >= NGLOBAL)
    fatal("%u: error: global count limit reached", lLine);
  if (contains(sym, sGlobalTable, sGlobalCount) >= 0)
    fatal("%u: error: global already defined", lLine);

  sGlobalTable[sGlobalCount] = sym;
  sGlobalCount++;
}

// a new function is being declared
void sFuncAdd(type_t type, symbol_t sym) {
  if (sFuncCount >= NFUNC)
    fatal("%u: error: function count limit reached", lLine);
  if (contains(sym, sFuncTable, sFuncCount) >= 0)
    fatal("%u: error: function already defined", lLine);

  sFuncTable[sFuncCount] = sym;       // save symbol
  sFuncPos  [sFuncCount] = cCodeLen;  // code position
  sFuncCount++;
}

// a new argument is being declared
void sArgAdd(type_t type, symbol_t sym) {
  if (sArgCount >= NARG)
    fatal("%u: error: argument count limit reached", lLine);
  if (contains(sym, sArgTable, sArgCount) >= 0)
    fatal("%u: error: duplicate arguments", lLine);

  sArgTable[sArgCount] = sym;
  sArgCount++;
}

// a new local has been declared
void sLocalAdd(type_t type, symbol_t sym) {
  if (sLocalCount >= NLOCAL)
    fatal("%u: error: local count limit reached", lLine);
  if (contains(sym, sLocalTable, sLocalCount) >= 0)
    fatal("%u: error: local already defined", lLine);

  sLocalTable[sLocalCount] = sym;
  sLocalCount++;
}

// return true if current token is a type
bool tIsType() {
  switch (tToken) {
  case TOK_CHAR:
  case TOK_INT:
  case TOK_VOID: return true;
  default:       return false;
  }
}

// lookup a symbol and push its value onto the stack
void sPushSymbol(symbol_t s) {
  int i;
  if ((i = contains(s, sLocalTable, sLocalCount)) >= 0) {
    cEmit1(INS_GETAL, i);
    return;
  }
  if ((i = contains(s, sArgTable, sArgCount)) >= 0) {
    cEmit1(INS_GETAA, i);
    return;
  }
  if ((i = contains(s, sGlobalTable, sGlobalCount)) >= 0) {
    cEmit1(INS_GETAG, i);
    return;
  }
  if ((i = contains(s, sFuncTable, sFuncCount)) >= 0) {
    cEmit1(INS_CONST, sFuncPos[i]);
    return;
  }
  fatal("%u: error: unknown identifier", lLine);
}

// consume a function call
void pExprCall(symbol_t sym) {
  int nargs = 0;
  while (!tFound(TOK_RPAREN)) {
    do {
      pExpr(1);
      nargs++;
    } while (tFound(TOK_COMMA));
  }
  sPushSymbol(sym);
  cEmit0(INS_CALL);
}

// consume a primary expression
// return true if lvalue else false
bool pExprPrimary() {
  token_t n = tNext();
  // parenthesized expression
  if (n == TOK_LPAREN) {
    bool lvalue = pExpr(1);
    tExpect(TOK_RPAREN);
    return lvalue;
  }
  // integer literal
  if (n == TOK_LITERAL) {
    cEmit1(INS_CONST, tSymValue);
    return false;
  }
  // idenfitier or function call
  if (n == TOK_SYMBOL) {
    // get symbol
    symbol_t sym = sIntern(tSym);
    // function call
    if (tFound(TOK_LPAREN)) {
      pExprCall(sym);
      return false;
    }
    else {
      // place symbol value on the stack
      sPushSymbol(sym);
      return true;
    }
  }

  fatal("%u: error: expected literal or identifier", lLine);
  return false;
}

// the precedence table
int pPrec(token_t c) {
  switch (c) {
  case TOK_ASSIGN: return 1;
  case TOK_LOGOR:  return 2;
  case TOK_LOGAND: return 3;
  case TOK_BITOR:  return 4;
//case TOK_BITXOR: ...
  case TOK_BITAND: return 5;
  case TOK_NEQU:
  case TOK_EQU:    return 6;
  case TOK_LTEQU:
  case TOK_GTEQU:
  case TOK_LT:
  case TOK_GT:     return 7;
//case TOK_SHR: ...
//case TOK_SHL: ...
  case TOK_ADD:
  case TOK_SUB:    return 8;
  case TOK_MOD:
  case TOK_MUL:
  case TOK_DIV:    return 9;
  }
  return 0;
}

// return true if this token is an operator
bool pIsOperator(token_t c) {
  return pPrec(c) > 0;
}

// precedence climbing expression parser
bool pExpr(int min_prec) {
  bool lvalue;
  // lhs
  lvalue = pExprPrimary();
  // while our operator is equal or higher precidence
  while (1) {
    // look ahead for possible operators
    token_t op = tPeek();
    if (!pIsOperator(op)) {
        break;
    }
    if (pPrec(op) < min_prec) {
        break;
    }
    // consume operator
    tNext();

    // dereference if needed
    if (op == TOK_ASSIGN) {
      if (!lvalue) {
        fatal("%u: error: assignment requires lvalue", lLine);
      }
    }
    else {
      if (lvalue) {
        cEmit0(INS_DEREF);
      }
    }

    // rhs
    pExpr(pPrec(op));

    // apply operator
    cEmit0(op);
    lvalue = false;
  }

  // ensure evaluated expression is rvalue
  if (lvalue) {
    cEmit0(INS_DEREF);
  }
  return false;
}

type_t pType() {
  token_t type = tNext();
  if (!tIsType()) {
    fatal("%u: error: type expected", lLine);
  }
  return type;
}

void pStmtIf() {
                                  // if
  tExpect(TOK_LPAREN);            // (
  pExpr(1);                       // <expr>
  tExpect(TOK_RPAREN);            // )

  cEmit1(INS_CONST, 0);
  int L0 = cEmit1(INS_JEQ, -1);

  pStmt();                        // <stmt>

  if (tFound(TOK_ELSE)) {         // else
    int L1 = cEmit1(INS_JMP, -1);
    cPatch(L0, cCodeLen);
    pStmt();                      // <stmt>
    cPatch(L1, cCodeLen);
  }
  else {
    cPatch(L0, cCodeLen);
  }
}

void pStmtWhile() {

  int L0 = cCodeLen;

                          // while
  tExpect(TOK_LPAREN);    // (
  pExpr(1);               // <expr>
  tExpect(TOK_RPAREN);    // )

  cEmit1(INS_CONST, 0);
  int L1 = cEmit1(INS_JEQ, -1);

  pStmt();                // <stmt>

  cEmit1(INS_JMP, L0);
  cPatch(L1, cCodeLen);
}

void pStmtReturn() {
                          // return
  pExpr(1);               // <expr>
  tExpect(TOK_SEMI);      // ;
  cEmit0(INS_RETURN);
}

void pStmtDo() {

  int L0 = cCodeLen;

                          // do
  pStmt();                // <stmt>
  tExpect(TOK_WHILE);     // while
  tExpect(TOK_LPAREN);    // (
  pExpr(1);               // <expr>
  tExpect(TOK_RPAREN);    // )
  tExpect(TOK_SEMI);      // ;

  cEmit1(INS_CONST, 0);
  cEmit1(INS_JNEQ, L0);
}

void pStmt() {
  // if statement
  if (tFound(TOK_IF)) {
    pStmtIf();
    return;
  }
  // return statement
  if (tFound(TOK_RETURN)) {
    pStmtReturn();
    return;
  }
  // while statement
  if (tFound(TOK_WHILE)) {
    pStmtWhile();
    return;
  }
  // do while statement
  if (tFound(TOK_DO)) {
    pStmtDo();
    return;
  }
  // compound statement
  if (tFound(TOK_LBRACE)) {
    while (!tFound(TOK_RBRACE)) {
      pStmt();
    }
    return;
  }
  // empty statement
  if (tFound(TOK_SEMI)) {
    return;
  }
  // expression
  pExpr(1);
  tExpect(TOK_SEMI);
  // rvalue not used
  cEmit0(INS_DROP);
}

void pParseGlobal(type_t type, symbol_t sym) {
  sGlobalAdd(type, sym);
  tExpect(TOK_SEMI);
}

void pParseLocal() {
  type_t   type = pType();
  token_t  name = tNext();
  symbol_t sym  = sIntern(tSym);
  sLocalAdd(type, sym);
  tExpect(TOK_SEMI);
}

void pParseFunc(type_t type, symbol_t sym) {

  // new scope so clear args and locals
  sArgCount   = 0;
  sLocalCount = 0;

  // record new function
  sFuncAdd(type, sym);

  // parse arguments
  if (!tFound(TOK_RPAREN)) {
    do {
      type_t  type = pType();
      token_t name = tNext();
      symbol_t sym = sIntern(tSym);
      sArgAdd(type, sym);
    } while (tFound(TOK_COMMA));
    tExpect(TOK_RPAREN);
  }

  // function body
  tExpect(TOK_LBRACE);

  // parse locals
  for (;;) {
    tPeek();
    if (!tIsType()) {
      break;
    }
    pParseLocal();
  }

  // allocate space for locals
  cEmit1(INS_ALLOC, sLocalCount);

  // parse statements
  while (!tFound(TOK_RBRACE)) {
    pStmt();
  }

  // return from function
  cEmit1(INS_CONST, 0);
  cEmit0(INS_RETURN);
}

void pParse() {
  while (!tFound(TOK_EOF)) {

    // parse a type
    type_t type = pType();

    // parse a name
    tExpect(TOK_SYMBOL);
    symbol_t sym = sIntern(tSym);

    // if a function decl 
    if (tFound(TOK_LPAREN)) {
      pParseFunc(type, sym);
    }
    else {
      pParseGlobal(type, sym);
    }
  }
}

// emit to output code stream
void cEmit0(int ins) {
  if (cCodeLen >= NCODELEN)
    fatal("%u: error: code limit reached", lLine);
  cCode[cCodeLen++] = ins;
}

// emit instruction and operand to code stream
int cEmit1(int ins, int opr) {
  cEmit0(ins);
  cEmit0(opr);
  return cCodeLen - 1;
}

// patch a previous operand
void cPatch(int loc, int opr) {
  cCode[loc] = opr;
}

#define DASM0(INS, NAME) \
  case INS: printf("%2u  %s\n", i, NAME); i += 1; break;

#define DASM1(INS, NAME) \
  case INS: printf("%2u  %s %u\n", i, NAME, opr); i += 2; break;

void dasm() {
  int i=0;
  while (i < cCodeLen) {
    int ins = cCode[i+0];
    int opr = cCode[i+1];
    switch (ins) {
    DASM0(INS_DEREF,  "DEREF");
    DASM0(INS_CALL,   "CALL ");
    DASM1(INS_CONST,  "CONST");
    DASM1(INS_GETAG,  "GETAG");
    DASM1(INS_GETAL,  "GETAL");
    DASM1(INS_GETAA,  "GETAA");
    DASM1(INS_ALLOC,  "ALLOC");
    DASM0(TOK_ASSIGN, "ASSIGN");
    DASM0(TOK_OR,     "OR");
    DASM0(TOK_AND,    "AND");
    DASM0(TOK_ADD,    "ADD");
    DASM0(TOK_SUB,    "SUB");
    DASM0(TOK_MUL,    "MUL");
    DASM0(TOK_DIV,    "DIV");
    DASM0(TOK_EQU,    "EQU");
    DASM0(TOK_LOGOR,  "LOGOR");
    DASM0(TOK_LOGAND, "LOGAND");
    DASM0(TOK_BITOR,  "BITOR");
    DASM0(TOK_MOD,    "MOD");
    DASM0(TOK_LT,     "LT");
    DASM0(TOK_GT,     "GT");
    DASM0(TOK_LTEQU,  "LTEQU");
    DASM0(TOK_GTEQU,  "GTEQU");
    DASM0(TOK_NEQU,   "NEQU");
    DASM0(TOK_LOGNOT, "LOGNOT");
    DASM0(INS_RETURN, "RETURN");
    DASM1(INS_JMP,    "JMP");
    DASM1(INS_JEQ,    "JEQ");
    DASM1(INS_JNEQ,   "JNEQ");
    DASM0(INS_DROP,   "DROP");
    default:
      fatal("Unknown instruction %u at %u", ins, i);
    }
  }
}

int main(int argc, char **args) {

  // line counting starts at 1
  lLine = 1;

  if (argc <= 1) {
    fatal("%u: error: argument expected", lLine);
  }

  // open input file for reading
  inFile = fopen(args[1], "r");
  if (!inFile) {
    return 1;
  }

  // discard first read (lK0 invalid)
  lNext();

// int L0 = cEmit1(INS_JMP, -1);

  // start parsing
  pParse();

  // jump to main at start
// int sym = sIntern("main");
// int i = contains(sym, sFuncIndex, sFuncCount);
// if (i < 0) { fatal("error: main() not defined"); }
// cPatch(L0, sFuncPos[i]);

  dasm();

  return 0;
}
