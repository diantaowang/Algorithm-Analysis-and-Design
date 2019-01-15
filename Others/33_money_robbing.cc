#include <cstdio>
#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
    int a;
    vector<uint32_t> s;
    while (cin >> a) {
        s.push_back(a);
        if (cin.get() == '\n')
            break;
    }
    vector<uint64_t> opt(2, 0);
    auto n = s.size();
    for (int i = 0; i < n; ++i) {
        opt.push_back(max(opt[i] + s[i], opt[i + 1]));
    }
    cout << opt.back() << endl;
        
    return 0;
}

