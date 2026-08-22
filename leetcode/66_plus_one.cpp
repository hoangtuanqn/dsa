#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
vector<int> plusOne(vector<int> &digits)
{
    int n = digits.size();
    int memo = 1;

    for (int i = n - 1; i >= 0 && memo; --i)
    {
        if (digits[i] + memo >= 10)
        {
            digits[i] = (digits[i] + memo) - 10;
            memo = 1;
        }
        else
        {
            digits[i] += memo;
            memo = 0;
        }
    }
    if (memo)
    {
        digits.push_back(0);
        digits[0] = 1;
    }

    return digits;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    // vector<int> arr = {4, 3, 2, 1};
    vector<int> arr = {9, 9, 9, 9};
    for (auto v : plusOne(arr))
    {
        cout << v << " ";
    }

    return 0;
}
