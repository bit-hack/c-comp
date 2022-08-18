// call a number of functions with arguments

int func1(int a) {
    return a;
}

int func2(int a, int b) {
    return a + b * 3;
}

int main() {
    return func1(1) + func2(1+2, 2+3);
}
