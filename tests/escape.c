// check the escape values are parsed correctly

int main() {

    if (*"\0" !=  0) return 1;
    if (*"\n" != 10) return 2;
    if (*"\r" != 13) return 3;
    if (*"\t" !=  9) return 4;
    if (*"\\" != 92) return 5;

    return 13;
}
