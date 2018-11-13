#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b)
{
    return (*(int *)a) > (*(int *)b);
}

int main(int argc, char *argv[])
{
    int m[1000000], b[1000000];
    int i = 0;
    do {
        scanf("%d", &m[i++]);
    } while (getchar() != '\n');
    int n = i;
    for (i = 0; i < n; ++i)
        scanf("%d", &b[i]);
    qsort(m, n, sizeof(int), cmp);
    qsort(b, n, sizeof(int), cmp);
    int max = abs(m[0] - b[0]);
    for (i = 1; i < n; ++i) {
        int t = abs(m[i] - b[i]);
        if (max < t)
            max = t;
    }
    printf("%d\n", max);
    return 0;
}
