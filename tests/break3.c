int main() {

    int i, j;

    i = 0;
    while (i < 5) {
        putchar('a');

        j = 0;
        while (j < 5) {
            putchar('b');

            j++;
        }

        if (i > 2) {
            break;
        }

        ++i;
    }

    putchar('\n');
}
