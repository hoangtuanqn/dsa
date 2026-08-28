#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
// có lời giải chi tiết ở: "132_palindrome_partitioning_II_using_DP.md", đây là biến thể từ bài xâu con đối xứng dài nhất
int expansion_from_center(const string& s) {
    int n = s.size();
    vector<int> dp(n + 1, INT_MAX);
    dp[0] = -1;
    int startI = 0, maxLen = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= 1; j++) {
            int l = i, r = i + j;
            while (l >= 0 && r < n && s[l] == s[r]) {
                if (r - l + 1 > maxLen) {
                    maxLen = r - l + 1;
                    startI = l;
                }
                dp[r + 1] = min(dp[r + 1], dp[l] + 1);
                --l;
                ++r;
            }
        }
    }
    return dp[n];
}
int minCut(string s) {
    return expansion_from_center(s);
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s;
    cin >> s;
    cout << expansion_from_center(s);

    return 0;
}
