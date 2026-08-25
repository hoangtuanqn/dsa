#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

// Có hd ở: 5_longest_palindromic_substring_using_expansion_from_center.md
string longestPalindrome(string s) {
    int startI = 0, maxLength = 1;
    int n = s.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= 1; ++j) {
            int l = i, r = i + j;
            while (l >= 0 && r < n && s[l] == s[r]) {
                if (r - l + 1 > maxLength) {
                    maxLength = r - l + 1;
                    startI = l;
                }
                --l;
                ++r;
            }
        }
    }
    return s.substr(startI, maxLength);
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s = "AAABCCD";
    cout << longestPalindrome(s);

    return 0;
}
