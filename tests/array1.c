int test(int *a, int *b) {
    a[1] = 12;
    return b[1];
}

int main() {
    int x[4];
    return test(x, x);
}
