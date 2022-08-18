int main() {
    int x;
    int *y;

    y = &x;

    x = 1234;

    // increment dereferenced value
    *y = *y + 1;

    return x;  // check it persists
}
