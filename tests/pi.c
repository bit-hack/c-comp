int main() {
    int r[2801];
    int i, k;
    int b, d;
    int c;

    c = 0;

    for (i = 0; i < 2800; ++i) {
        r[i] = 2000;
    }

    for (k = 2800; k > 0; k = k - 14) {
        d = 0;

        i = k;
        for (;;) {
            d = d + r[i] * 10000;
            b = 2 * i - 1;

            r[i] = d % b;
            d = d / b;
            --i;
            if (i == 0) break;
            d = d * i;
        }
        printf("%d", c + d / 10000);
        c = d % 10000;
    }

    printf("\n");
    return 0;
}
