int glob[2];

int a() {
    return glob[1];
}

int b(int *x) {
    return x[1];
}

int c() {
    int *x;
    x = glob;
    return x[1];
}

int main() {
    glob[1] = 1234;
//    return a();
//    return b(glob);
    return c();
}
