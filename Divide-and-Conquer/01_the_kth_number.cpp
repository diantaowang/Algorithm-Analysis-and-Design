#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>

using std::vector;
using namespace::std;

int main()
{
    int n, k, t;
    scanf("%d%d", &n, &k);
    k = n - k + 1;
    vector<int> A;
    for (int i = 0; i < n; ++i) {
        scanf("%d", &t);
        A.push_back(t);
    }
    auto b = A.begin(), e = A.end();
    nth_element(b, b + k - 1, e);
    printf("%d\n", A[k - 1]);
    return 0;
}
