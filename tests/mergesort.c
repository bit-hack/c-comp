int seed;
int arr[16];

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

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int l, int m, int r)
{
	int L[32], R[32];
	int i, j, k;
	int n1, n2;

	n1 = m - l + 1;
	n2 = r - m;

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; ++i)
		L[i] = arr[l + i];
	for (j = 0; j < n2; ++j)
		R[j] = arr[m + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while ((i < n1) && (j < n2)) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			++i;
		}
		else {
			arr[k] = R[j];
			++j;
		}
		++k;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1) {
		arr[k] = L[i];
		++i;
		++k;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2) {
		arr[k] = R[j];
		++j;
		++k;
	}
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(int l, int r)
{
  int m;

	if (l < r) {
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		m = l + (r - l) / 2;

		// Sort first and second halves
		mergeSort(l, m);
		mergeSort(m + 1, r);

		merge(l, m, r);
	}
}

int main() {

  int i;

  arr[0] = 151;
  arr[1] = 22;
  arr[2] = 15;
  arr[3] = 3;
  arr[4] = 96;
  arr[5] = 37;
  arr[6] = 58;
  arr[7] = 86;
  arr[8] = 22;
  arr[9] = 74;

  mergeSort(0, 9);

  for (i = 0; i < 10; ++i) {
    number(arr[i]);
    putchar(10);
  }

  return 0;
}