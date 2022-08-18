int test(int a) {
    *(&a) = 1234;
    return a;
}

int main() {
    return test(1);
}
