#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

// sử dụng DP
string longestPalindrome(string s)
{
    int n = s.size(), maxLenght = 1, start = 0;
    vector<vector<bool>>
        dp(n, vector<bool>(n, false));
    dp[n - 1][n - 1] = true;
    FOR(i, 0, n - 1)
    {
        if (s[i] == s[i + 1])
        {
            dp[i][i + 1] = true;
            maxLenght = 2;
            start = i;
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
                if (len > maxLenght)
                {
                    maxLenght = len;
                    start = i;
                }
            }
        }
    }
    return s.substr(start, maxLenght);
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    // vector<int> arr = {6, 7, 8, 5, 4, 1, 2, 3};
    string text = "aacabdkacaa";
    cout << longestPalindrome(text);

    return 0;
}
