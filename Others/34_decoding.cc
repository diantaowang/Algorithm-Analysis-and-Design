#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>

#define UP  '2' * 10 + '6'
#define LOW '0' * 10 + '0'

using namespace std;

int main(int argc, char *argv[])
{
    int m;
    string s("00"), t;
    vector < int >opt(2, 1);
    getline(cin, t);
    s += t;
    auto n = s.length();
    for (unsigned long i = 2; i < n; ++i) {
        if (s[i] == '0') {
            int t = s[i - 1] * 10 + s[i];
            if (t == LOW || t > UP) {
                cout << 0 << endl;
                return 0;
            } else {
                opt.push_back(opt[i - 1]);
            }
        } else {
            if (s[i - 1] == '0') {
                opt.push_back(opt[i - 1]);
            } else if (s[i - 1] * 10 + s[i] > UP) {
                opt.push_back(opt[i - 1]);
            } else {
                opt.push_back(opt[i - 1] + opt[i - 2]);
            }
        }
    }
    cout << opt.back() << endl;
    return 0;
}
