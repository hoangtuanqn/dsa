#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
string longestCommonPrefix(vector<string> &strs)
{
    sort(strs.begin(), strs.end());
    string first = strs[0];
    string last = strs.back();
    int i = 0;
    while (i < first.size() && last[i] == first[i])
    {
        i++;
    }

    return first.substr(0, i);
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<string> arr = {""};
    cout << longestCommonPrefix(arr);
    // PINALSIGYAHRPI
    // PINALSIGYAHRPPII
    return 0;
}
