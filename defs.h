#pragma once

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
#define TOK_EQU     10  // ==
#define TOK_LOGOR   11  // ||
#define TOK_BITOR   12  // |
#define TOK_LOGAND  13  // &&
#define TOK_BITAND  14  // &
#define TOK_SEMI    15  // ;
#define TOK_LPAREN  16  // (
#define TOK_RPAREN  17  // )
#define TOK_COMMA   18  // ,
#define TOK_LBRACE  19  // {
#define TOK_RBRACE  20  // }
#define TOK_MOD     21  // %
#define TOK_LT      22  // <
#define TOK_GT      23  // >
#define TOK_LTEQU   24  // <=
#define TOK_GTEQU   25  // >=
#define TOK_NEQU    26  // ==
#define TOK_LOGNOT  27  // !=
#define TOK_SYMBOL  128
#define TOK_LITERAL 129
#define TOK_IF      130 // if
#define TOK_INT     131 // int
#define TOK_RETURN  132 // return
#define TOK_ELSE    133 // else
#define TOK_CHAR    134 // char
#define TOK_VOID    135 // void
#define TOK_WHILE   136 // while
#define TOK_DO      137 // do

#define INS_DEREF   256 + 0   // dereference
#define INS_CONST   256 + 1   // constant
#define INS_CALL    256 + 2   // 
#define INS_GETAG   256 + 3   // get address global
#define INS_GETAL   256 + 4   // get address local
#define INS_GETAA   256 + 5   // get address argument
#define INS_ALLOC   256 + 6   // allocate space for locals
#define INS_RETURN  256 + 7   // return from function
#define INS_JMP     256 + 8   // jump unconditional
#define INS_JEQ     256 + 9   // jump if equal
#define INS_JLT     256 + 10  // jump if less than
#define INS_JLTE    256 + 11  // jump if less than or equal to
#define INS_JNEQ    256 + 12  // jump if not equal
#define INS_JGT     256 + 13  // jump if greater than
#define INS_JGTE    256 + 14  // jump if greater than or equal to
#define INS_DROP    256 + 15  // drop top of stack

#define NFUNC       32
#define NGLOBAL     32
#define NARG        8
#define NLOCAL      32
#define SYMTABLEN   (1024*4)
#define NCODELEN    (1024*4)

#define token_t     int
#define symbol_t    int
#define type_t      int

#define bool        int
#define true        1
#define false       0

void  fatal   (char *msg, ...);
bool  pExpr   (int v);
void  pStmt   ();
void  cEmit0  (int c);
int   cEmit1  (int c, int opr);
void  cPatch  (int loc, int opr);
bool  strmatch(char *a, char *b);
char *strskip (char *c);
char *strcopy (char *dst, char *src);
int   strint  (char *a);
int   contains(symbol_t find, symbol_t *arr, int count);
