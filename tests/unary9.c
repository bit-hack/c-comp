int inc(int *x, int *y, int z) {
    *x = *y + z;
}

int main() {
    int x;
    int y;

    x = 123;
    y = 20;

    inc(&x, &y, 3);

    return x;
}