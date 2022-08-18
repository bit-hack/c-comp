// check we can parse nested while loops

int main() {
    if (1) {
        while (2) {
            ;
        }
    }

    while (3) {
        while (4) {
            if (5) {
                ;
            }
        }
    }

    return 1;
}