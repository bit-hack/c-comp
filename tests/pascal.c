
int number(int v) {
    int x = v % 10;
    if (v) {
      number(v / 10);
      putchar('0' + x);
    }
}

int binomialCoeff(int n, int k)
{
    int res=1, i;
    if (k > n - k)
        k = n - k;
    for (i = 0; i < k; i=i+1)
    {
        res = res * (n - i);
        res = res / (i + 1);
    }
      
    return res;
}

void printPascal(int n)
{
    int line, i;
    // Iterate through every line and
    // print entries in it
    for (line = 0; line < n; line=line+1)
    {
        // Every line has number of 
        // integers equal to line 
        // number
        for (i = 0; i <= line; i=i+1) {
            number(binomialCoeff(line, i));
            putchar(' ');
        }
        putchar(10);
    }
}

// Driver program 
int main()
{
    int n = 7;
    printPascal(n);
    return 0;
}
