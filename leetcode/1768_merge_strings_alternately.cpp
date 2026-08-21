#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
string mergeAlternately(string word1, string word2)
{
    string rs;
    int i = 0, j = 0;
    while (i < word1.size() || j < word2.size())
    {
        if (i < word1.size())
            rs += word1[i++];
        if (j < word2.size())
            rs += word2[j++];
    }
    return rs;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> arr = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << mergeAlternately("abc", "pqrrr");
    return 0;
}
