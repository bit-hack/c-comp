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
    int x;
    int y;
    int z;
    glob[1] = 1234;
    x = a();
    y = b(glob);
    z = c();
    return x + y + z;
}
