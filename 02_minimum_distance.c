#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct point {
	double x, y;
};

int cmpx(const void *a, const void *b)
{
	struct point *p = (struct point *)a;
	struct point *q = (struct point *)b;
	if (fabs(p->x - q->x) < 0.0000001) {
		if (fabs(p->y - q->y) < 0.0000001)
			return 0;
		else if (p->y > q->y)
			return 1;
		else
			return -1;
	} else if (p->x > q->x) {
		return 1;
	} else {
		return -1;
	}
}

int cmpy(const void *a, const void *b)
{
	return ((struct point *)a)->y - ((struct point *)b)->y;
}

double min(double a, double b, double c)
{
	double min;
	if (a < b)
		min = a;
	else
		min = b;
	if (c < min)
		return c;
	else
		return min;
}

double distance(struct point a, struct point b)
{
	double t1 = (a.x - b.x) * (a.x - b.x);
	double t2 = (a.y - b.y) * (a.y - b.y);
	return sqrt(t1 + t2);
}

void CopyData(struct point *a, struct point *b, int *n)
{
	a->x = b->x;
	a->y = b->y;
	++(*n);
}

int division(struct point *Yl, struct point *Yr, struct point *Y,
	     int n, double pivot, double base_y, int left_part_n)
{
	int count_l = 0, count_r = 0;
	for (int i = 0; i < n; ++i) {
		if (Y[i].x < pivot)
			CopyData(&Yl[count_l], &Y[i], &count_l);
		else if (Y[i].x > pivot)
			CopyData(&Yr[count_r], &Y[i], &count_r);
		else if (Y[i].y <= base_y) {
			if (count_l + 1 > left_part_n)
				return 0;
			else
				CopyData(&Yl[count_l], &Y[i], &count_l);
		} else
			CopyData(&Yr[count_r], &Y[i], &count_r);
	}
	return 1;
}

int MergeSort(struct point *Yl, struct point *Yr, struct point *A,
	      int num_l, int num_r, double pivot, double min_dist)
{
	int cnt = 0, i, j;
	for (i = 0, j = 0; i < num_l && j < num_r;) {
		if (Yl[i].y <= Yr[j].y) {
			if (Yl[i].x + min_dist >= pivot)
				CopyData(&A[cnt], &Yl[i], &cnt);
			++i;
		} else {
			if (Yr[j].x <= pivot + min_dist)
				CopyData(&A[cnt], &Yr[j], &cnt);
			++j;
		}
	}
	for (; i < num_l; ++i) {
		if (Yl[i].x + min_dist >= pivot)
			CopyData(&A[cnt], &Yl[i], &cnt);
	}
	for (; j < num_r; ++j) {
		if (Yr[j].x <= pivot + min_dist)
			CopyData(&A[cnt], &Yr[j], &cnt);
	}
	return cnt;
}

double ClosestPair(struct point *X, struct point *Y, int l, int r)
{
	if (r - l == 1)
		return distance(X[l], X[r]);
	else if (r - l == 2) {
		double d0 = distance(X[l], X[l + 1]);
		double d1 = distance(X[l], X[r]);
		double d2 = distance(X[l + 1], X[r]);
		return min(d0, d1, d2);
	}
	int l0 = l, r0 = (l + r) / 2;
	int l1 = r0 + 1, r1 = r;
	int num_l = r0 - l0 + 1;
	int num_r = r1 - l1 + 1;
	double pivot = (X[r0].x + X[l1].x) / 2;
	// divide Y[] to Yl[] and Yr[] by pivot.
	struct point *Yl = (struct point *)malloc(sizeof(struct point) * num_l);
	struct point *Yr = (struct point *)malloc(sizeof(struct point) * num_r);
	if (!division(Yl, Yr, Y, r - l + 1, pivot, X[r0].y, num_l))
		return 0.0;
	double min_distance = ClosestPair(X, Yl, l0, r0);
	double min_distance_r = ClosestPair(X, Yr, l1, r1);
	if (min_distance_r < min_distance)
		min_distance = min_distance_r;
	// merge sort
	struct point *A =
	    (struct point *)malloc(sizeof(struct point) * (r - l + 1));
	int num = MergeSort(Yl, Yr, A, num_l, num_r, pivot, min_distance);
	for (int i = 0; i < num - 1; ++i) {
		for (int j = i + 1; j < i + 8 && j < num; ++j) {
			double d = distance(A[i], A[j]);
			if (d < min_distance)
				min_distance = d;
		}
	}
	free(Yl);
	free(Yr);
	free(A);
	return min_distance;
}

int main(int argc, char *argv[])
{
	int n;
	scanf("%d", &n);
	struct point *X = (struct point *)malloc(sizeof(struct point) * n);
	struct point *Y = (struct point *)malloc(sizeof(struct point) * n);
	for (int i = 0; i < n; ++i) {
		scanf("%lf%lf", &X[i].x, &X[i].y);
		Y[i].x = X[i].x;
		Y[i].y = X[i].y;
	}
	qsort(X, n, sizeof(struct point), cmpx);
	qsort(Y, n, sizeof(struct point), cmpy);
	printf("%.2lf\n", ClosestPair(X, Y, 0, n - 1));
	return 0;
}
