int seed;

int rand() {
  if (seed == 0)
    seed = 54321;
  return seed = (seed * 1103515245 + 12345) & 268435455;
}

int number(int v) {
  int x;
  x = v % 10;
  if (v) {
    number(v / 10);
    putchar('0' + x);
  }
}

void quicksort(int *number, int first, int last) {

  int i, j, pivot, temp;

  if (first < last) {
    pivot = first;
    i = first;
    j = last;

    while (i < j) {
      while (number[i] <= number[pivot] && i < last)
        i = i + 1;
      while (number[j] > number[pivot])
        j = j - 1;
      if (i < j) {
        temp = number[i];
        number[i] = number[j];
        number[j] = temp;
      }
    }

    temp = number[pivot];
    number[pivot] = number[j];
    number[j] = temp;
    quicksort(number, first, j - 1);
    quicksort(number, j + 1, last);
  }
}

int main() {

  int test[16];
  int i;

  for (i = 0; i < 16; i = i + 1) {
    test[i] = rand() % 256;
  }

  quicksort(test, 0, 15);

  for (i = 0; i < 16; i = i + 1) {
    number(test[i]);
    putchar(10);
  }

  return 0;
}