#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int strStr(string haystack, string needle)
{
    int n = haystack.size(), m = needle.size();
    if (n < m)
        return -1;
    for (int start = 0; start <= n - m; ++start)
    {
        bool matched = true;
        for (int l = start; l < start + m; ++l)
        {
            if (haystack[l] != needle[l - start])
            {
                matched = false;
                break;
            }
        }
        if (matched)
            return start;
    }
    return -1;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    cout << strStr("a", "a");
    return 0;
}
