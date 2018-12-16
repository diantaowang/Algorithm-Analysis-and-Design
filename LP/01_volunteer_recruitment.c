#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MIN 0.000001
#define INF 0x7fffffff
#define _INF 0x80000000

// e is the index of new basis in matrix A (begin with 0);
// l means that B[l] will be replace by e (begin with 0);
// it's sure that the A[l][e] != 0;
void pivot(int *B, double **A, double *b, double *c,
       double *z, int e, int l, int m, int n)
{
    //  
    if (fabs(A[l][e] - 1.0) > MIN) {
        double factor = A[l][e];
        for (int j = 0; j < n; ++j)
            A[l][j] = A[l][j] / factor;
        b[l] = b[l] / factor;
    }
    // 
    for (int i = 0; i < m; ++i) {
        if (i != l) {
            double factor = A[i][e];
            b[i] -= factor * b[l];
            for (int j = 0; j < n; ++j) {
                A[i][j] -= factor * A[l][j];
            }
        }
    }
    double factor = c[e];
    *z -= factor * b[l];
    for (int j = 0; j < n; ++j) {
        c[j] -= factor * A[l][j];
    }
    B[l] = e;
}

// Input Parameter: 
// A is the m*n original coefficient matrix;
// B[0] is the position of the 1-st column vertor of unit matrix in matrix A...
// b is the 1*m column vertor;
// c is the 1*n column vector;
// z is the value of "transpose(c) * x";
// m -> the number of equations; 
// n -> the number of all variable include slack variables.
// Return Value:
// 0 -> LP is infeasible;
// 1 -> found the initial simplex. 
int initial_simplex(int *B, double **A, double *b,
            double *c, double *z, int m, int n)
{
    // let l be the index of the minimum bi
    int l = 0;
    double bl = b[0];
    for (int i = 1; i < m; ++i) {
        if (bl > b[i]) {
            bl = b[i];
            l = i;
        }
    }
    // set B to include the indices of slack variables.
    for (int i = m - 1, idx = n; i >= 0; --i)
        B[i] = --idx;
    *z = 0;

    if (bl >= 0)
        return 1;
    // Construct Laux by adding -x0, but this way may add cahce miss.
    // Another method is the n-th column vertor has alreadly been filled -1.0,
    // but we never use the n-th column vertor before this step.
    for (int i = 0; i < m; ++i)
        A[i][n] = -1.0;
    double c_laux[n + 1];
    for (int j = 0; j < n; ++j)
        c_laux[j] = 0.0;
    c_laux[n] = 1.0;
    pivot(B, A, b, c_laux, z, n, l, m, n + 1);
    // 
    while (1) {
        int end = 1;
        int e = 0, l = 0;
        double minc = 0.0, theta = INF;
        for (int j = 0; j < n + 1; ++j) {
            if (end && c_laux[j] < 0)
                end = 0;
            if (minc > c_laux[j]) {
                minc = c_laux[j];
                e = j;
            }
        }
        if (end) {
            // calculatex()
            break;
        }
        for (int i = 0; i < m; ++i) {
            if (A[i][e] > 0) {
                double t = b[i] / A[i][e];
                if (theta > t) {
                    theta = t;
                    l = i;
                }
            }
        }
        // infeasible.
        // the step can be cancel, because the auxiliary linear program Laux
        // must has a feasible solution. 
        if (theta == INF)
            return 0;
        pivot(B, A, b, c_laux, z, e, l, m, n + 1);
    }
    if (fabs(*z) < MIN) {
        for (int i = 0; i < m; ++i) {
            int idx = B[i];
            if (fabs(c[idx]) >= MIN) {
                double factor = c[idx];
                *z -= factor * b[i];
                for (int j = 0; j < n; ++j) {
                    c[j] -= factor * A[i][j];
                }
            }
        }
        return 1;
    }
    return 0;
}

void calculatex(int *B, double *b, double *x, int m, int n)
{
    for (int j = 0; j < n; ++j)
        x[j] = 0;
    for (int i = 0; i < m; ++i)
        x[B[i]] = b[i];
}

int simplex(double **A, double *b, double *c,
        double *z, double *x, int m, int n)
{
    int *B = (int *)malloc(m * sizeof(int));
    if (!initial_simplex(B, A, b, c, z, m, n))
        return 0;
    while (1) {
        int end = 1;
        int e = 0, l = 0;
        double minc = 0.0, theta = INF;
        for (int j = 0; j < n; ++j) {
            if (end && c[j] < 0)
                end = 0;
            if (minc > c[j]) {
                minc = c[j];
                e = j;
            }
        }
        if (end) {
            calculatex(B, b, x, m, n);
            *z = -*z;
            return 1;
        }
        for (int i = 0; i < m; ++i) {
            if (A[i][e] > 0) {
                double t = b[i] / A[i][e];
                if (theta > t) {
                    theta = t;
                    l = i;
                }
            }
        }
        if (theta == INF)
            return 0;
        pivot(B, A, b, c, z, e, l, m, n);
    }
}

double A[200][1210];

int main(int argc, char *argv[])
{
    int m, n;
    double z = 0, b[200];
    double x[1210], c[1210];
    scanf("%d%d", &m, &n);
    for (int i = 0, t; i < m; ++i) {
        scanf("%d", &t);
        b[i] = -(double)t;
    }
    for (int j = 0; j < n; ++j) {
        int s, f, ci;
        scanf("%d%d%d", &s, &f, &ci);
        c[j] = (double)ci;
        for (int i = s - 1; i < m && i < f; ++i)
            A[i][j] = -1;
    }
    for (int j = n; j < n + m; ++j)
        c[j] = 0.0;
    for (int j = 0; j < n + m; ++j)
        x[j] = 0.0;
    for (int i = 0; i < m; ++i)
        A[i][i + n] = 1;
    double *H[200];
    for (int i = 0; i < m; ++i)
        H[i] = &A[i][0];
    if (simplex(H, b, c, &z, x, m, n + m))
        printf("%.0lf\n", z);
    return 0;
}
