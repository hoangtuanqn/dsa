#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
char findTheDifference(string s, string t)
{
    int val[26] = {0};
    int n = s.size();
    FOR(i, 0, n)
    {
        val[s[i] - 'a']++;
    }
    FOR(i, 0, n + 1)
    {
        val[t[i] - 'a']--;
        if (val[t[i] - 'a'] == -1)
        {
            return t[i];
        }
    }

    return '\0';
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cout << findTheDifference("", "y");
    return 0;
}
