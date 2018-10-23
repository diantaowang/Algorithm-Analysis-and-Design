#include <stdio.h>

int find_upper_bound(int *A, int l, int r, int element)
{
    while (1) {
        if (l == r)
            return l;
        int mid = (l + r) / 2;
        if (element <= A[mid])
            r = mid;
        else 
            l = mid + 1;
    }
}

int main(int argc, char *argv[])
{
    int t, n, a[100010], subseqence[100010];
    scanf("%d", &t);
    for (int i = 0; i < t; ++i) {
        scanf("%d", &n);
        for (int j = 0; j < n; ++j)
            scanf("%d", &a[j]);
        int longest = 1;
        subseqence[0] = a[0];
        for (int j = 1; j < n; ++j) {
            if (a[j] > subseqence[longest - 1])
                subseqence[longest++] = a[j];
            else {
                int pos = find_upper_bound(subseqence, 0, longest - 1, a[j]);
                subseqence[pos] = a[j];
            }
        }
        printf("%d\n", longest);
    } 
    return 0;
}
