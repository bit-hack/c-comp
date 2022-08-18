// test a combination of ifs and if elses can be parsed

int test1(int a) {
    if (a)
        return 1;
    else
        return 2;
}

int test2(int a) {
    if (a)
        return 1;
}

int test3(int a, int b) {
    if (a)
        if (b)
            return 1;
}

int test4(int a, int b) {
    if (a)
        if (b)
            return 1;
        else
            return 2;
}

int test5(int a, int b) {
    if (a)
        if (b)
            return 1;
        else
            return 2;
    else
        return 3;
}

int main() {
    return test1(1)    || 
           test2(1)    ||
           test3(1, 2) ||
           test4(1, 2) ||
           test5(1, 2);
}
