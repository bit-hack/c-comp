int main( ) {
    int i, n;
    i = 0;
    n = 0;
    // no initial or inc expr
    for (; i < 3;) {
        n = n + 1;
        i = i + 1;
    }
    return n;
}