int main( ) {
    char *str;
    int i;
    str = "TestString";
    for (i=0; str[i]; ++i) {
        putchar(str[i]);
    }
    putchar(10);
}
