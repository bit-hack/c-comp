int main() {
    int i;
    while (i < 8) {
        if ((++i % 2) == 0)
            continue;
        printf("%d\n", i);
    }
}
