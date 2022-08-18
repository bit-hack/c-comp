int main() {
    int x;
    int *y;
    y = &x;

    x = 1234;

    return *y;
}
