#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

int romanToInt(string s)
{
    int total = 0;
    unordered_map<char, int> maps = {
        {'I', 1},
        {'V', 5},
        {'X', 10},
        {'L', 50},
        {'C', 100},
        {'D', 500},
        {'M', 1000},
    };
    int i = 0;
    while (i < s.size())
    {
        if (s[i] == 'I' && (s[i + 1] == 'X' || s[i + 1] == 'V'))
        {
            total += maps[s[i + 1]] - maps[s[i]];
            i += 2;
        }
        else if (s[i] == 'X' && (s[i + 1] == 'L' || s[i + 1] == 'C'))
        {
            total += maps[s[i + 1]] - maps[s[i]];
            i += 2;
        }
        else if (s[i] == 'C' && (s[i + 1] == 'D' || s[i + 1] == 'M'))
        {
            total += maps[s[i + 1]] - maps[s[i]];
            i += 2;
        }
        else
        {
            total += maps[s[i]];
            i++;
        }
    }
    if (i == s.size() - 1)
        total += maps[s[i]];
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
