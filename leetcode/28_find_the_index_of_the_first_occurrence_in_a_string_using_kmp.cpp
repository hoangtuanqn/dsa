#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
vector<int> build_lps(string pattern)
{
    int n = pattern.size();
    vector<int> lps(n, 0);
    int i = 1;
    int len = 0;
    while (i < n)
    {
        if (pattern[len] == pattern[i])
        {
            ++len;
            lps[i] = len;
            ++i;
        }
        else
        {
            if (len != 0)
                len = lps[len - 1];
            else if (i < n)
                ++i;
        }
    }
    return lps;
}
int kmp_search(string text, string pattern)
{
    int n = text.size();
    int m = pattern.size();
    int ans = -1;
    if (m == 0 || m > n)
        return ans;
    vector<int> lps = build_lps(pattern);
    int i = 0, j = 0;
    while (i < n)
    {
        if (text[i] == pattern[j])
        {
            i++;
            j++;
        }
        else
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                ++i;
            }
        }
        if (j == m)
        {
            return i - m;
        }
    }
    return ans;
}
int strStr(string haystack, string needle)
{
    return kmp_search(haystack, needle);
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string text = "AAAABCDAAA", pattern = "AABCD";
    // FOR(i, 0, pattern.size())
    // {
    //     cout << pattern[i] << " ";
    // }
    // cout << endl;
    // for (auto v : build_lps(pattern))
    // {
    //     cout << v << " ";
    // }
    // cout << endl;
    cout << "rs: " << strStr(text, pattern);
    return 0;
}
