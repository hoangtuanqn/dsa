#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
// worse case: abcbab
// nếu s[i] != s[j]. Phải kiểm tra cả 2 nhánh, nếu bỏ i thì có dc k? nếu bỏ j thì có dc k?
bool isPalin(const string& s, int l, int r) {
    while (l < r) {
        if (s[l] != s[r])
            return false;
        ++l;
        --r;
    }
    return true;
}
bool validPalindrome(const string& s) {
    int l = 0, r = s.size() - 1;
    while (l < r) {
        if (s[l] != s[r]) {
            if (isPalin(s, l + 1, r) || isPalin(s, l, r - 1)) {
                return true;
            } else {
                return false;
            }
        }
        ++l;
        --r;
    }
    return true;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s;
    cin >> s;
    cout << (validPalindrome(s) ? "true" : "false");

    return 0;
}
