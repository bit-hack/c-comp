int main() {
    int i;
    for (i=0; i<8; ++i) {
        if ((i % 2) == 0)
            continue;
        printf("%d\n", i);
    }
}
