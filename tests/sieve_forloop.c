// run sieve using a global array

int number(int v) {
    int x;
    x = v % 10;
    if (v) {
      number(v / 10);
      putchar('0' + x);
    }
}

int prime[33];

void SieveOfEratosthenes(int n)
{
    int p, i;

    for (p=2; p * p <= n; ++p) {
        if (prime[p] == 0) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.

            for (i=p*p; i <= n; i=i+p) {
                prime[i] = 1;
            }
        }
    }

    // Print all prime numbers
    for (p=2; p <= n; ++p) {
        if (prime[p] == 0) {
            number(p);
            putchar(10);
        }
    }
}

int main( ) {
    SieveOfEratosthenes(32);
}
