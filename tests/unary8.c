int inc(int *a) {
    *a = *a + 1;
}

int test(int a) {
    inc(&a);
    return a;
}

int main() {
    return test(1);
}
