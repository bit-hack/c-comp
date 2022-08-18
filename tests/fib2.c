// calculate next fibbonaci value

int main() {
  int t1;
  int t2;
  int nextTerm;
  int n;
  
  t1       = 0;
  t2       = 1;
  nextTerm = t1 + t2;
  n        = 13;

  while (nextTerm <= n) {
    t1       = t2;
    t2       = nextTerm;
    nextTerm = t1 + t2;
  }

  return nextTerm;
}
