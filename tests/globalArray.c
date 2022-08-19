int g[2];
int x;

int main() {
    *(g)   = 54;
    *(g+1) = 1234;

    if (x != 0) {
        return -1;
    }

    return *(g+1);
}
