void printSierpinski(int n)
{
    int y;
    int x;
    int i;

    for (y = n - 1; y >= 0; y = y - 1) {
 
        // printing space till
        // the value of y
        for (i = 0; i < y; i = i + 1) {
            putchar(' ');
        }
 
        // printing '*'
        for (x = 0; x + y < n; x = x + 1) {
 
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
    int n;
    n = 16;
 
    // Function calling
    printSierpinski(n);
 
    return 0;
}
