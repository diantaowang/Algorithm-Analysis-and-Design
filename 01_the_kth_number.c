#include <stdio.h>
#include <stdlib.h>

int pivot(int *A, int l, int r);
int selectk(int *A, int l, int r, int k);

void swap(int *a, int *b)
{
	int t;
	t = *a;
	*a = *b;
	*b = t;
}

// insert sort: return position
int sort1_5(int *A, int l, int r)
{
	int len = r - l + 1;
	if (len == 1)
		return l;
	for (int i = l + 1, j; i <= r; ++i) {
		int t = A[i];
		for (j = i - 1; j >= l && t > A[j]; --j)
			A[j + 1] = A[j];
		A[j + 1] = t;
	}
	return l + (len - 1) / 2;
}

// return position 
int partition1_5(int *A, int l, int r)
{
	int len = r - l + 1;
	if (len == 5) {
		if (A[l] < A[l + 1])
			swap(&A[l], &A[l + 1]);
		if (A[l + 2] < A[l + 3])
			swap(&A[l + 2], &A[l + 3]);
		if (A[l] < A[l + 2]) {
			swap(&A[l], &A[l + 2]);
			swap(&A[l + 1], &A[l + 3]);
		}
		if (A[l + 1] < A[l + 4])
			swap(&A[l + 1], &A[l + 4]);
		if (A[l + 1] < A[l + 2]) {
			swap(&A[l + 1], &A[l + 2]);
			swap(&A[l + 3], &A[l + 4]);
		}
		if (A[l + 4] < A[l + 2])
			return l + 2;
		else
			return l + 4;

	} else if (len == 4) {
		if (A[l] < A[l + 1])
			swap(&A[l], &A[l + 1]);
		if (A[l + 2] < A[l + 3])
			swap(&A[l + 2], &A[l + 3]);
		if (A[l] < A[l + 2]) {
			swap(&A[l], &A[l + 2]);
			swap(&A[l + 1], &A[l + 3]);
		}
		if (A[l + 1] > A[l + 2])
			return l + 1;
		else
			return l + 2;
	} else if (len == 3) {
		if (A[l] < A[l + 1])
			swap(&A[l], &A[l + 1]);
		if (A[l + 2] > A[l])
			return l;
		else if (A[l + 2] < A[l + 1])
			return l + 1;
		else
			return l + 2;
	} else if (len == 2) {
		if (A[l] > A[r])
			return l;
		else
			return r;
	} else
		return l;
}

int pivot(int *A, int l, int r)
{
	if (r - l < 5) {
		return partition1_5(A, l, r);
		// return sort1_5(A, l, r);
	}
	int cnt = 0;
	for (int i = l; i <= r; i += 5, ++cnt) {
		int right = i + 4;
		if (right > r)
			right = r;
		int m = partition1_5(A, i, right);
		// int m = sort1_5(A, i, right);
		swap(&A[m], &A[l + cnt]);
	}
	// select the (l + (cnt + 1) / 2)-th number in array A[],
	// not the (cnt + 1 / 2)-th number in the array A[].
	return selectk(A, l, l + cnt - 1, l + (cnt + 1) / 2);
}

int partition(int *A, int l, int r, int p)
{
	int pivot = A[p];
	swap(&A[p], &A[r]);
	int i = l;
	for (int j = l; j < r; ++j)
		if (A[j] > pivot) {
			swap(&A[i], &A[j]);
			++i;
		}
	swap(&A[i], &A[r]);
	return i;
}

int selectk(int *A, int l, int r, int k)
{
	while (1) {
		if (l > r)
			exit(0);
		if (l == r)
			return l;
		int p = pivot(A, l, r);
		int pos = partition(A, l, r, p);
		if (k - 1 == pos)
			return k - 1;
		else if (k - 1 < pos)
			r = pos - 1;
		else
			l = pos + 1;
	}
}

int main(int argc, char *argv[])
{
	int n, k;
	scanf("%d%d", &n, &k);
	int *A = (int *)malloc(sizeof(int) * n);
	for (int i = 0; i < n; ++i)
		scanf("%d", &A[i]);
	int index = selectk(A, 0, n - 1, k);
	printf("%d\n", A[index]);
	return 0;
}
