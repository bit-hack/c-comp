// check we can parse nested while loops

int main() {

    int x;
    x = 0;

    if (1) {
        while (x) {
            ;
        }
    }

    while (10 * 0) {
        while (1 - 1) {
            if (5) {
                ;
            }
        }
    }

    return 1;
}