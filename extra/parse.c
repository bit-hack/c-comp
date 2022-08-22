int curloc;
int lexval;
int token;
int thechar;
int nglob;
int nlocal;
int nfun;
int strsize;
int narg;
int curgloboffs;
int nsym;
int pusharg;
int pushop;
int argids[50];
int globids[300];
int globoffs[300];
int globscalar[300];
int funids[300];
int funoffs[300];
int localids[50];
char symbol[30000];
char code[10000];
char names[30000];
int eputstr(char *s) {
  while (*s)
    putchar(*s++);
}
int error(char *s) {
  int i;
  eputstr(s);
  eputstr(" at: ");
  i = 0;
  while (i < 20) {
    putchar(getchar());
    i++;
  }
  eputstr("\n");
  exit(1);
}
int digit(int c) {
  return '0' <= c && c <= '9';
}
int letter(int c) {
  return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_' || digit(c);
}
int eqstr(char *p, char *q) {
  while (*p) {
    if (*p++ != *q++)
      return 0;
  }
  return !(*q);
}
int lookup(char *name) {
  int i;
  int j;
  char *ns;
  ns = names;
  i = 0;
  while (i < nsym) {
    if (eqstr(ns, name)) {
      return i;
    }
    while (*ns++);
    i++;
  }
  while (*ns++ = *name++);
  j = nsym++;
  return j;
}
int next() {
  int r;
  r = thechar;
  thechar = getchar();
  return r;
}
int gobble(int t, int rr, int r) {
  if (thechar == t) {
    next();
    return rr;
  }
  return r;
}
int getstring(int delim) {
  int c;
  strsize = 0;
  while ((c = next()) != delim) {
    if (c == '\\') {
      if ((c = next()) == 'n')
        c = '\n';
      else if (c == 't')
        c = '\t';
      else if (c == '0')
        c = 0;
    }
    symbol[strsize++] = c;
  }
  symbol[strsize++] = 0;
}
int instr(char *s, int c) {
  while (*s) {
    if (*s++ == c)
      return 1;
  }
  return 0;
}
int getlex() {
  int c;
  char *p;
  while (0 <= (c = next()) && c <= ' ');
  if (c == -1 || instr("()[]{},;", c)) {
    return c;
  }
  if (c == '/') {
    if (thechar == '*') {
      while (next() != '*' || thechar != '/');
      next();
      return getlex();
    } else
      return ((11 * 100) + 15);
  }
  if (c == '*') return ((11 * 100) + 14);
  if (c == '%') return ((11 * 100) + 16);
  if (c == '-') return gobble(c, ((12 * 100) + 25), ((10 * 100) + 13));
  if (c == '>') return gobble('=', ((8 * 100) + 11), ((8 * 100) + 26));
  if (c == '<') return gobble('=', ((8 * 100) + 24), ((8 * 100) + 8));
  if (c == '=') return gobble(c, ((8 * 100) + 6), ((1 * 100) + 1));
  if (c == '+') return gobble(c, ((12 * 100) + 19), ((10 * 100) + 12));
  if (c == '!') return gobble('=', ((8 * 100) + 7), ((12 * 100) + 17));
  if (c == '&') return gobble(c, ((4 * 100) + 3), ((7 * 100) + 5));
  if (c == '|') return gobble(c, ((3 * 100) + 2), ((5 * 100) + 4));
  if (c == '\'') {
    getstring(c);
    lexval = symbol[0];
    return 257;
  }
  if (c == '"') {
    getstring(c);
    return 258;
  }
  if (digit(c)) {
    lexval = c - '0';
    while (digit(thechar)) {
      lexval = lexval * 10 + next() - '0';
    }
    return 257;
  }
  if (letter(c)) {
    p = symbol;
    *p++ = c;
    while (letter(thechar))
      *p++ = next();
    *p = 0;
    if ((lexval = lookup(symbol)) < 7) {
      if (lexval == 6)
        return 517;
      return lexval + 512;
    }
    return 256;
  }
  error("Bad input");
}
int istoken(int t) {
  if (token == t) {
    token = getlex();
    return 1;
  }
  return 0;
}
int expect(int t) {
  if (!istoken(t)) {
    error("token expected");
  }
}
int type() {
  expect(517);
  while (istoken(((11 * 100) + 14)));
}
int name() {
  int r;
  if (token != 256)
    error("name expected");
  r = lexval;
  token = getlex();
  return r;
}
int emit(int opc) {
  code[curloc++] = opc;
}
int emitat(int a, int c) {
  code[a++] = c;
  code[a] = c / 256;
}
int emitop(int rator, int rand) {
  int r;
  emit(rator + 7);
  r = curloc;
  emit(rand);
  emit(rand / 256);
  return r;
}
int pushloop(int puop, int max, int *arr) {
  int i;
  i = 0;
  pushop = puop;
  while (i < max) {
    if (arr[i] == lexval) {
      pusharg = i;
      return 1;
    }
    i++;
  }
  return 0;
}
int pushval() {
  int lval;
  lval = 1;
  if (pushloop((16 * 4), nlocal, localids)) {
  } else if (pushloop((16 * 2), narg, argids)) {
    pusharg = narg - pusharg - 1;
  } else if (pushloop((16 * 6), nglob, globids)) {
    lval = globscalar[pusharg];
    pusharg = globoffs[pusharg];
  } else {
    lval = 0;
    if (pushloop((16 * 12), nfun, funids)) {
      pusharg = funoffs[pusharg];
    } else if (lexval < 7 + 4) {
      pusharg = lexval - 7;
    } else {
      error("undefined variable");
    }
  }
  emitop(pushop, pusharg);
  return lval;
}
int pderef(int l) {
  if (l)
    emit(23);
}
int expr(int needval, int prec) {
  int na;
  int islval;
  int jdst;
  int op;
  int any;
  int opprec;
  islval = 0;
  if (istoken(257)) {
    emitop((16 * 8 ), lexval);
  } else if (istoken(258)) {
    emitop((16 * 9 ), strsize);
    any = 0;
    while (any < strsize)
      emit(symbol[any++]);
  } else if (istoken(256)) {
    islval = pushval();
  } else if (istoken('(')) {
    islval = expr(0, 0);
    expect(')');
  } else if (istoken(((12 * 100) + 17))) {
    expr(1, 100);
    emit(17);
  } else if (istoken(((10 * 100) + 13))) {
    expr(1, 100);
    emit(18);
  } else if (istoken(((11 * 100) + 14))) {
    expr(1, 100);
    islval = 1;
  } else if (istoken(((7 * 100) + 5))) {
    if (expr(0, 100) == 0)
      error("lvalue required");
  } else
    error("syntax error in expr");
  any = 1;
  while (any) {
    op = ((token) % 100);
    if (istoken('(')) {
      pderef(islval);
      na = 0;
      if (!istoken(')')) {
        do {
          expr(1, 0);
          na++;
        } while (istoken(','));
        expect(')');
      }
      emitop((16 * 11 ), na * 2);
      islval = 0;
    } else if (istoken('[')) {
      pderef(islval);
      expr(1, 0);
      emit(12);
      expect(']');
      islval = 1;
    } else if (istoken(((12 * 100) + 19)) || istoken(((12 * 100) + 25))) {
      if (!islval)
        error("no lval for ++");
      emit(op);
      islval = 0;
    } else
      any = 0;
  }
  opprec = ((token) / 100);
  while (prec < opprec) {
    op = ((token) % 100);
    if (op != 1) {
      pderef(islval);
    } else {
      if (!islval)
        error("no lval for =");
    }
    if (istoken(((4 * 100) + 3)) || istoken(((3 * 100) + 2))) {
      emit(21);
      if (op == 2)
        emit(17);
      jdst = emitop((16 * 14), 0);
      emit(22);
      expr(1, opprec);
      emitat(jdst, curloc);
    } else {
      token = getlex();
      expr(1, opprec);
      emit(op);
    }
    islval = 0;
    opprec = ((token) / 100);
  }
  if (needval) {
    pderef(islval);
    islval = 0;
  }
  return islval;
}
int pexpr() {
  expect('(');
  expr(1, 0);
  expect(')');
}
int stmt() {
  int jdest;
  int tst;
  if (istoken('{')) {
    while (!istoken('}'))
      stmt();
    return 1;
  }
  if (istoken(513)) {
    pexpr();
    jdest = emitop((16 * 14), 0);
    stmt();
    if (istoken(514)) {
      tst = emitop((16 * 13 ), 0);
      emitat(jdest, curloc);
      stmt();
      emitat(tst, curloc);
    } else {
      emitat(jdest, curloc);
    }
    return 1;
  }
  if (istoken(515)) {
    tst = curloc;
    pexpr();
    jdest = emitop((16 * 14), 0);
    stmt();
    emitop((16 * 13 ), tst);
    emitat(jdest, curloc);
    return 1;
  }
  if (istoken(516)) {
    jdest = curloc;
    stmt();
    expect(515);
    pexpr();
    emit(17);
    emitop((16 * 14), jdest);
    return 1;
  }
  if (istoken(512)) {
    expr(1, 0);
    expect(';');
    emit(10);
    return 1;
  }
  if (istoken(';')) {
    return 1;
  }
  {
    expr(1, 0);
    emit(22);
    expect(';');
    return 1;
  }
}
int parseFunc(int objid) {
  funids[nfun] = objid;
  funoffs[nfun++] = curloc;
  narg = 0;
  if (!istoken(')')) {
    do {
      type();
      argids[narg++] = name();
    } while (istoken(','));
    expect(')');
  }
  expect('{');
  nlocal = 0;
  while (token == 517) {
    type();
    do {
      localids[nlocal++] = name();
    } while (istoken(','));
    expect(';');
  }
  if (16 < nlocal)
    emitop((16 * 10 ), nlocal);
  while (!istoken('}'))
    stmt();
  emit(10);
  return 1;
}
int parseGlobalVar(int objid) {
  globoffs[nglob] = curgloboffs;
  if (istoken('[')) {
    expect(257);
    curgloboffs = curgloboffs + lexval;
    expect(']');
    globscalar[nglob] = 0;
  } else {
    curgloboffs++;
    globscalar[nglob] = 1;
  }
  globids[nglob++] = objid;
  expect(';');
  return 1;
}
int parse() {
  int objid;
  token = getlex();
  while (token >= 0) {
    type();
    objid = name();
    if (istoken('(')) {
      parseFunc(objid);
    } else {
      parseGlobalVar(objid);
    }
  }
  return 1;
}
int main() {
  int n;
  char *p;
  char *q;
  nsym = 7 + 4;
  p = names;
  q = "return\0if\0else\0while\0do\0int\0char\0getchar\0putchar\0eputchar\0exit";
  n = 62;
  do
    *p++ = *q++;
  while (n--);
  curloc = 10;
  thechar = getchar();
  parse();
  n = curloc;
  curloc = 0;
  lexval = lookup("main");
  pushval();
  emitop((16 * 11 ), 0);
  emit(27);
  putchar(n);
  putchar(n / 256);
  p = code;
  while (n--) {
    putchar(*p++);
  }
  return 0;
}
