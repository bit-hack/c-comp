int seed;
int rand() {
    if (seed == 0)
        seed = 54321;
    return seed = (seed * 1103515245 + 12345) & 268435455;
}

int number(int v) {
    int x;
    x = v % 10;
    if (v) {
      number(v / 10);
      putchar('0' + x);
    }
}

int main() {

    int i;
    int n;

    for (i=0; i<20; i = i + 1) {
        n = rand();

        number(n);
        putchar(10);
    }

    return 0;
}
