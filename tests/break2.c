int main() {

    int i, j;

    i = 0;
    while (i < 5) {
        j = 0;
        while (j < 5) {
            putchar('.');

            if (j > 1) {
                break;
            }

            j++;
        }
        ++i;
    }

    putchar('\n');
}
