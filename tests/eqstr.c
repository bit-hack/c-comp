int eqstr(char *p, char *q) {
  while (*p) {
    if (*p++ != *q++)
      return 0;
  }
  return !(*q);
}

int main( ) {

    char *a;
    char *b;

    a = "int";
    b = "int";

    return eqstr(a, b);
}