#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"

int cCode[NCODELEN];  // code stream
int cCodeLen;         // code length

int main() {

  cCodeLen = fread(cCode, 4, NCODELEN, stdin);
  if (ferror(stdin)) {
    fatal("stdin error");
  }

  int i=0;
  while (i < cCodeLen) {
    i += dasm(cCode + i, i);
    printf("\n");
  }

  return 0;
}
