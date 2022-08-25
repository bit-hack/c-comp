

int main() {
    int i;
    do {
        if ((++i % 2) == 0)
            continue;  // jump to evaluation at the bottom
        printf("%d\n", i);
    }
    while (i < 8);
}
