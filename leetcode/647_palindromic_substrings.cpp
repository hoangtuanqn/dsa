#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
// sử dụng DP
int countSubstrings(string s)
{
    int n = s.size(), count = s.size();
    vector<vector<bool>> dp(n, vector<bool>(n, false));
    dp[n - 1][n - 1] = true;

    // bước cơ sở
    FOR(i, 0, n - 1)
    {
        if (s[i] == s[i + 1])
        {
            count++;
            dp[i][i + 1] = true;
        }
        dp[i][i] = true;
    }
    for (int len = 3; len <= n; ++len)
    {
        for (int i = 0; i <= n - len; ++i)
        {
            int j = i + len - 1;
            if (s[i] == s[j] && dp[i + 1][j - 1])
            {
                dp[i][j] = true;
                ++count;
            }
        }
    }
    return count;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string text = "aaa";
    cout << countSubstrings(text);
    return 0;
}
