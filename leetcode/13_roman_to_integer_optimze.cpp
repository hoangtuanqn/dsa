#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

int romanToInt(string s)
{
    int total = 0;
    unordered_map<char, int> val = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000},
    };
    int n = s.size();
    FOR(i, 0, n)
    {
        if (i + 1 < n && val[s[i]] < val[s[i + 1]])
            total -= val[s[i]];
        else
            total += val[s[i]];
    }
    return total;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    // vector<int> arr = {4, 5, 32, 1, 2, 3, 9, 2, -3, 4};
    // sort(arr.begin(), arr.end());
    // cout << search(arr, 32);
    cout << romanToInt("XII");

    return 0;
}
