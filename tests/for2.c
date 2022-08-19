int main( ) {
    int i, n;
    i = 0;
    n = 0;
    // no initial expr
    for (; i < 3; i = i + 1) {
        n = n + 1;
    }
    return n;
}