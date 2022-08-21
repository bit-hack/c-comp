int floorSqrt(int x)
{
    int i;
    int result;

    // Base cases
    if (x == 0 || x == 1)
    return x;
 
    // Starting from 1, try all numbers until
    // i*i is greater than or equal to x.
    i = 1;
    result = 1;
    while (result <= x)
    {
        ++i;
        result = i * i;
    }
    return i - 1;
}

int min(int a, int b){
	if(b>a)
    	return a;
	else
	    return b;
}

int jumpsearch(int *arr, int x, int n)
{
    int step, prev;

	// Finding block size to be jumped
	step = floorSqrt(n);

	// Finding the block where element is
	// present (if it is present)
	prev = 0;
	while (arr[min(step, n)-1] < x)
	{
		prev = step;
		step = step + floorSqrt(n);
		if (prev >= n)
			return -1;
	}

	// Doing a linear search for x in block
	// beginning with prev.
	while (arr[prev] < x)
	{
		++prev;

		// If we reached next block or end of
		// array, element is not present.
		if (prev == min(step, n))
			return -1;
	}
	// If element is found
	if (arr[prev] == x)
		return prev;

	return -1;
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
	index = jumpsearch(arr, x, n);

    return index;
}
