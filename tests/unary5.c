int child(int *v) {
    *v = 4321;
}

int main() {
    int x;
    x = 2;
    child(&x);
    return x;  // check it persists
}
