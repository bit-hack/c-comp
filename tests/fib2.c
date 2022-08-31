// calculate next fibbonaci value

int main() {
  
  int t1       = 0;
  int t2       = 1;
  int nextTerm = t1 + t2;
  int n        = 13;

  while (nextTerm <= n) {
    t1       = t2;
    t2       = nextTerm;
    nextTerm = t1 + t2;
  }

  return nextTerm;
}
