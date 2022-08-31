void printSierpinski(int n)
{
    int y, x, i;

    for (y = n - 1; y >= 0; --y) {
 
        // printing space till
        // the value of y
        for (i = 0; i < y; ++i) {
            putchar(' ');
        }
 
        // printing '*'
        for (x = 0; x + y < n; ++x) {
 
        // printing '*' at the appropriate position
        // is done by the and value of x and y
        // wherever value is 0 we have printed '*'
        if(x & y) {
            putchar(' '); putchar(' ');
        }
        else {
            putchar('*'); putchar(' ');
        }
        }

        putchar(10);
    }
}
 
// Driver code
int main()
{
    int n = 16;
 
    // Function calling
    printSierpinski(n);
 
    return 0;
}
