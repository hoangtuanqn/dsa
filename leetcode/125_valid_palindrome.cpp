#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
bool isPalindrome(string s)
{
    FOR(i, 0, s.size())
    s[i] = tolower(s[i]);

    int l = 0, r = s.size() - 1;
    while (l < r)
    {
        while (l < r && !isalnum(s[l]))
            l++;
        while (l < r && !isalnum(s[r]))
            r--;
        if (s[l] != s[r])
            return false;
        ++l;
        --r;
    }
    return true;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> arr = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << isPalindrome("0P");
    return 0;
}
