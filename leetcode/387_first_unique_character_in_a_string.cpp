// 977. Squares of a Sorted Array
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
int firstUniqChar(string s)
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int count[26] = {0}, n = s.size();
    FOR(i, 0, n)
    {
        count[s[i] - 'a']++;
    }
    FOR(i, 0, n)
    {
        if (count[s[i] - 'a'] == 1)
        {
            return i;
        }
    }
    return -1;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    cout << firstUniqChar("aabb");

    return 0;
}
