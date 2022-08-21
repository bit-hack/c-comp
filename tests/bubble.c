int number(int v) {
  int x;
  x = v % 10;
  if (v) {
    number(v / 10);
    putchar('0' + x);
  }
}

void swap(int* xp, int* yp)
{
	int temp;
  temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int *arr, int n)
{
	int i, j;
	for (i = 0; i < n - 1; ++i)
		// Last i elements are already in place
		for (j = 0; j < n - i - 1; ++j)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
}

/* Function to print an array */
void printArray(int *arr, int size)
{
	int i;
	for (i = 0; i < size; ++i) {
        number(arr[i]);
        putchar(10);
    }
}

// Driver program to test above functions
int main()
{
    int arr[7];
    arr[0] = 64;
    arr[1] = 34;
    arr[2] = 25;
    arr[3] = 12;
    arr[4] = 22;
    arr[5] = 11;
    arr[6] = 90;

	bubbleSort(arr, 7);
	printArray(arr, 7);
}
