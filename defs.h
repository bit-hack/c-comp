#pragma once

#define TOK_EOF     0         // \0
#define TOK_ASSIGN  1         // =
#define TOK_ADD     2         // +
#define TOK_SUB     3         // -
#define TOK_MUL     4         // *
#define TOK_DIV     5         // /
#define TOK_INC     6         // ++
#define TOK_DEC     7         // --
#define TOK_LOGOR   8         // ||
#define TOK_BITOR   9         // |
#define TOK_LOGAND  10        // &&
#define TOK_BITAND  11        // &
#define TOK_SEMI    12        // ;
#define TOK_LPAREN  13        // (
#define TOK_RPAREN  14        // )
#define TOK_COMMA   15        // ,
#define TOK_LBRACE  16        // {
#define TOK_RBRACE  17        // }
#define TOK_MOD     18        // %
#define TOK_LT      19        // <
#define TOK_GT      20        // >
#define TOK_LTEQU   21        // <=
#define TOK_GTEQU   22        // >=
#define TOK_EQU     23        // ==
#define TOK_NEQU    24        // !=
#define TOK_LOGNOT  25        // !
#define TOK_BITNOT  26        // ~
#define TOK_LBRACK  27        // [
#define TOK_RBRACK  28        // ]

#define TOK_SYMBOL  32
#define TOK_INTLIT  33        // integer literal
#define TOK_STRLIT  34        // string literal

#define TOK_IF        64  + 0   // if
#define TOK_INT       64  + 1   // int
#define TOK_RETURN    64  + 2   // return
#define TOK_ELSE      64  + 3   // else
#define TOK_CHAR      64  + 4   // char
#define TOK_VOID      64  + 5   // void
#define TOK_WHILE     64  + 6   // while
#define TOK_DO        64  + 7   // do
#define TOK_FOR       64  + 8   // for
#define TOK_BREAK     64  + 9   // break
#define TOK_CONTINUE  64  + 10  // continue

#define INS_DEREF   128 + 0   // dereference
#define INS_CONST   128 + 1   // constant
#define INS_CALL    128 + 2   // 
#define INS_GETAG   128 + 3   // get address of global
#define INS_GETAL   128 + 4   // get address of local
#define INS_GETAA   128 + 5   // get address of argument
#define INS_ALLOC   128 + 6   // allocate space for locals
#define INS_RETURN  128 + 7   // return from function
#define INS_JMP     128 + 8   // jump unconditional
#define INS_JZ      128 + 9   // jump if zero
#define INS_JNZ     128 + 10  // jump if not zero
#define INS_DROP    128 + 11  // drop top of stack
#define INS_SCALL   128 + 12  // system call
#define INS_NEG     128 + 13  // unary minus
#define INS_INDEX   128 + 14  // array subscript
#define INS_DUP     128 + 15  // duplicate
#define INS_SWAP    128 + 16  // swap top items on the stack
#define INS_STRTAB  128 + 17  // set string table location
#define INS_STR     128 + 18  // get address of string
#define INS_LINE    128 + 19  // source code line

#define NFUNC       32
#define NGLOBAL     32
#define NARG        8
#define NLOCAL      32
#define SYMTABLEN   (1024*4)
#define NCODELEN    (1024*4)
#define NSTRTABLEN  (1024*4)
#define NBREAKS     8
#define NCONTINUES  8
#define NLINES      1024

#define token_t     int
#define symbol_t    int
#define type_t      int

#define bool        int
#define true        1
#define false       0

void  fatal   (char *msg, ...);
int   dasm    (int *cCode, int loc);

bool  strMatch(char *a, char *b);
char *strSkip (char *c);
char *strCopy (char *dst, char *src);
int   strToInt(char *a);
int   contains(symbol_t find, symbol_t *arr, int count);

char *tokName(token_t tok);
