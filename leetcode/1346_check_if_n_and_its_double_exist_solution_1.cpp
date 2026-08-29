#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
bool isPalin(const string& s) {
    int l = 0, r = s.size() - 1;
    while (l < r) {
        if (s[l] != s[r]) {
            return false;
        }
        l++;
        r--;
    }
    return true;
}
int removePalindromeSub(const string& s) {
    if (isPalin(s)) {
        return 1;
    }
    for (int i = 0; i < s.size() - 1; ++i) {
        if (s[i] != s[i + 1]) {
            return 2;
        }
    }
    return 1;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s;
    cin >> s;
    cout << removePalindromeSub(s);
    return 0;
}
