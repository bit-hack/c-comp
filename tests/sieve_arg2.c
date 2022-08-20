// run sieve when the argument was a global array, which is now passed as a
// pointer.

int number(int v) {
    int x;
    x = v % 10;
    if (v) {
      number(v / 10);
      putchar('0' + x);
    }
}

void SieveOfEratosthenes(int *prime, int n)
{
    int p;
    int i;

    p = 2;

    while (p * p <= n) {
        if (prime[p] == 0) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.

            i = p * p;
            while (i <= n) {
                prime[i] = 1;
                i = i + p;
            }
        }

        p = p + 1;
    }

    p = 2;

    // Print all prime numbers
    while (p <= n) {
        if (prime[p] == 0) {
            number(p);
            putchar(10);
        }
        p = p + 1;
    }
}

int prime[33];

int main( ) {
    SieveOfEratosthenes(prime, 32);
}
