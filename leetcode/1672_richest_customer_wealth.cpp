// 1672. Richest Customer Wealth
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
int maximumWealth(vector<vector<int>> &accounts)
{
    int rs = 0;
    FOR(i, 0, accounts.size())
    {
        int amount = 0;
        FOR(j, 0, accounts[i].size())
        {
            amount += accounts[i][j];
        }
        rs = max(rs, amount);
    }
    return rs;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<vector<int>> a = {{1, 2, 3}, {3, 2, 1}};
    cout << maximumWealth(a);

    return 0;
}
