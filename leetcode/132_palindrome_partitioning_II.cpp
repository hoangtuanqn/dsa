#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
vector<vector<bool>> buildDP(const string& s) {
    int n = s.size();
    vector<vector<bool>> p = vector(n, vector<bool>(n, false));
    p[n - 1][n - 1] = true;
    FOR(i, 0, n - 1) {
        if (s[i] == s[i + 1]) {
            p[i][i + 1] = true;
        }
        p[i][i] = true;
    }

    for (int len = 3; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            if (s[i] == s[j] && p[i + 1][j - 1]) {
                p[i][j] = true;
            }
        }
    }
    return p;
}
int minCut(const string& s) {
    int n = s.size();
    vector<vector<bool>> isPal = buildDP(s);
    vector<int> dp(n + 1, INT_MAX);
    dp[0] = -1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (isPal[j][i - 1]) {
                dp[i] = min(dp[i], dp[j] + 1);
            }
        }
    }
    return dp[n];
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s;
    cin >> s;
    cout << minCut(s);
    return 0;
}
