#include <stdio.h>

struct {
	int n;
	int k[101];
} r[100][100];

void calc(int a[][100], int i, int j, int k)
{
	int n1 = r[i + 1][j].n;
	int n2 = r[i][j + 1].n;
	// make merge sorting easier.
	r[i + 1][j].k[n1] = -1;
	r[i][j + 1].k[n2] = -1;
	if (n1 + n2 < k)
		k = n1 + n2;
	for (int l = 0, t1 = 0, t2 = 0; l < k; ++l) {
		if (r[i + 1][j].k[t1] >= r[i][j + 1].k[t2]) {
			r[i][j].k[l] = r[i + 1][j].k[t1] + a[i][j];
			++t1;
		} else {
			r[i][j].k[l] = r[i][j + 1].k[t2] + a[i][j];
			++t2;
		}
	}
	r[i][j].n = k;
}

int main(int argc, char *argv[])
{
	int m, n, k;
	int a[100][100];
	scanf("%d%d%d", &m, &n, &k);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			scanf("%d", &a[i][j]);
		}
	}
	r[m - 1][n - 1].n = 1;
	r[m - 1][n - 1].k[0] = a[m - 1][n - 1];
	// i -> row, j -> column;
	for (int i = m - 2; i >= 0; --i) {
		r[i][n - 1].n = 1;
		r[i][n - 1].k[0] = r[i + 1][n - 1].k[0] + a[i][n - 1];
	}
	for (int j = n - 2; j >= 0; --j) {
		r[m - 1][j].n = 1;
		r[m - 1][j].k[0] = r[m - 1][j + 1].k[0] + a[m - 1][j];
	}
	int i = m - 2, j = n - 2;
	if (i >= 0 && j >= 0) {
		while (1) {
			calc(a, i, j, k);
			if (i == 0 && j == 0)
				break;
			// j unchanged -> column refresh.
			for (int x = i - 1; x >= 0; --x) {
				calc(a, x, j, k);
				if (x == 0 && j == 0)
					break;
			}
			// i unchanged -> row refresh.
			for (int y = j - 1; y >= 0; --y) {
				calc(a, i, y, k);
				if (y == 0 && i == 0)
					break;
			}
			if (i > 0)
				--i;
			if (j > 0)
				--j;
		}
	}
	for (int i = 0; i < k - 1; ++i)
		printf("%d ", r[0][0].k[i]);
	printf("%d\n", r[0][0].k[k - 1]);
	return 0;
}
