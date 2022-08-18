int global;

int main() {
    int *ptr;

    ptr = &global;

    global = 1234;

    return *ptr;
}
