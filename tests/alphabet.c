// little test of a syscall to check we can print
// out the alphabet

int main() {

    int i;

    while (i < 26) {
        putchar('a' + i);
        i = i + 1;
    }

    putchar(10);

    return 0;
}