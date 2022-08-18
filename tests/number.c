// a little more complex test using recursion and
// syscalls to print an integer number

int number(int v) {
    int x;
    x = v % 10;
    if (v) {
      number(v / 10);
    }
    putchar('0' + x);
}

int main() {
    number(12345);
    putchar(10);
    return 0;
}
