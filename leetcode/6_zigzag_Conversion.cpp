#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
string convert(string s, int numRows)
{
    string rs;
    int distance = numRows + max(0, numRows - 2);
    for (int row = 0; row < numRows; ++row)
    {
        for (int i = row; i < s.size(); i += distance)
        {
            int idx = i + (distance - row * 2);
            rs += s[i];
            if (row > 0 && idx < s.size() && idx != i)
            {
                rs += s[idx];
            }
        }
    }

    return rs;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cout << convert("ABC", 2);
    // PINALSIGYAHRPI
    // PINALSIGYAHRPPII
    return 0;
}
