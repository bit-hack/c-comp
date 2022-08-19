int func(int *x, int size) {
    while (size) {
        size = size - 1;
        x[size] = size;
    }
}

int global[16];

int main() {

    func(global, 16);

    return global[3];
}
