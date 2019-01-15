#include <iostream>
#include <algorithm>

using namespace::std;

int main(int argc, char *argv[])
{
    int n = 0, m = 1;
    cin >> n;
    while (n > 4) {
        m *= 3;
        n = n - 3;
    }
    m *= n;
    cout << m << endl;
    return 0;
}
