int* foo(int* a, int b) {
  return &a[1];
}

int main() {
    int a[2];
    int b;
    b = 11;
    *foo(a, b) = 10;
    return a[1];
}