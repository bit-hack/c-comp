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
