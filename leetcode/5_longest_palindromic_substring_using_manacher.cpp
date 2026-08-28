#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
string initial(string s) {
    string t = "^#";
    for (int i = 0; i < s.size(); ++i) {
        t += s[i];
        t += "#";
    }
    t += "$";
    return t;
}
vector<int> manacher(string s) {
    string t = initial(s);
    int n = t.size();
    int center = 0, rigthEdge = 0;
    vector<int> p(n, 0);
    for (int i = 1; i < n - 1; ++i) {
        if (i < rigthEdge) {
            int mirror = 2 * center - i;
            p[i] = min(rigthEdge - i, p[mirror]);
        }
        while (t[i - p[i] - 1] == t[i + p[i] + 1]) {
            p[i]++;
        }
        if (i + p[i] > rigthEdge) {
            rigthEdge = i + p[i];
            center = i;
        }
    }
    return p;
}
string longestPalindrome(string s) {
    auto p = manacher(s);
    int bestCenter = 0, maxLength = 1;
    for (int i = 1; i < p.size(); ++i) {
        if (p[i] > maxLength) {
            maxLength = p[i];
            bestCenter = i;
        }
    }
    int start = (bestCenter - maxLength) / 2;
    return s.substr(start, maxLength);
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cout << longestPalindrome("cbbd");
    return 0;
}
