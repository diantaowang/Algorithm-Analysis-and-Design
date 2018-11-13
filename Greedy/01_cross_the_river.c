#include <stdio.h>

int main(int argc, char *argv[])
{
    int n, w, s = 0, p[30001] = { 0 };
    scanf("%d%d", &n, &w);
    for (int i = 0; i < n; ++i) {
        int t;
        scanf("%d", &t);
        ++p[t];
    }
    for (int l = 1, r = 30000; l <= r;) {
        if (l + r > w) {
            s += p[r];
            p[r] = 0;
        } else {
            int min = p[l];
            if (min > p[r])
                min = p[r];
            p[l] -= min;
            if (l == r) {
                s += (min + 1) / 2;
            } else {
                s += min;
                p[r] -= min;
            }
        }
        if (p[l] == 0)
            ++l;
        if (p[r] == 0)
            --r;
    }
    printf("%d\n", s);
    return 0;
}
