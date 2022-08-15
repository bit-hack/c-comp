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
        return 'a';
    }
    else {
        return 3;
    }

    while (func1(1, 2)) {
        if (two_locals) {
            1234;
        }
    }

    do {
        if (one_local)
            one_local = 2;
    } while (1);

    return func1(one_local, two_locals);
}
