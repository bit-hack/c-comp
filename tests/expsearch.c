int min(int a, int b) {
    if (a < b) return a;
    return b;
}

// A recursive binary search function. It returns
// location of x in given array arr[l..r] is
// present, otherwise -1
int binarySearch(int *arr, int l, int r, int x)
{
    int mid;

	if (r >= l)
	{
		mid = l + (r - l)/2;

		// If the element is present at the middle
		// itself
		if (arr[mid] == x)
			return mid;

		// If element is smaller than mid, then it
		// can only be present n left subarray
		if (arr[mid] > x)
			return binarySearch(arr, l, mid-1, x);

		// Else the element can only be present
		// in right subarray
		return binarySearch(arr, mid+1, r, x);
	}

	// We reach here when element is not present
	// in array
	return -1;
}

// Returns position of first occurrence of
// x in array
int exponentialSearch(int *arr, int n, int x)
{
    int i;

	// If x is present at first location itself
	if (arr[0] == x)
		return 0;

	// Find range for binary search by
	// repeated doubling
	i = 1;
	while (i < n && arr[i] <= x)
		i = i*2;

	// Call binary search for the found range.
	return binarySearch(arr, i/2,
							min(i, n-1), x);
}

int main()
{
    int arr[16];
    int x;
    int n;
    int index;

    arr[0] = 0;
    arr[1] = 1;
    arr[2] = 1;
    arr[3] = 2;
    arr[4] = 3;
    arr[5] = 5;
    arr[6] = 8;
    arr[7] = 13;
    arr[8] = 21;
    arr[9] = 34;
    arr[10] = 55;
    arr[11] = 89;
    arr[12] = 144;
    arr[13] = 233;
    arr[14] = 377;
    arr[15] = 610;

	x = 55;
    n = 16;
	index = exponentialSearch(arr, n, x);

    return index;
}
