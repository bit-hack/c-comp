int x[8];
int y[8];

int main() {

    int n;
    int sum;

    sum = 0;
    n = 8;

    do {
        n = n - 1;
        y[n] = n;
    } while (n);

    while (n <= 7) {
        sum = sum + y[n];
        n = n + 1;
    }

    return sum;
}
