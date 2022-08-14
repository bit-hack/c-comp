int glob1;
int glob2;

int func1(int a, int b) {
    {
        return a + b + 3;
    }
}

int main() {
    int one_local;
    int two_locals;
    {
    }
    if (one_local) {
        return 2;
    }
    else {
        return 3;
    }

    if (two_locals) {
        1234;
    }

    return 0;
}
