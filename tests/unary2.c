// check that a parenthesized expression remains an lvalue

int main() {
    int x;
    return &(x);
}
