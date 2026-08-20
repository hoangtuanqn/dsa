#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int finonacci(int n, vector<int> &dp)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    if (dp[n] != -1)
        return dp[n];
    return dp[n] = finonacci(n - 1, dp) + finonacci(n - 2, dp);
}
int fib(int n)
{
    vector<int> dp(n + 1, -1);
    return finonacci(n, dp);
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cout << fib(4);
    return 0;
}
