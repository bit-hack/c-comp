int* foo(int* a) {
  return &a[1];
}

int main() {
    int a[2];
    *foo(a) = 10;
    return a[1];
}
