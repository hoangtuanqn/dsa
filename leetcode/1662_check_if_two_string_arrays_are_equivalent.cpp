#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
bool arrayStringsAreEqual(vector<string> &word1, vector<string> &word2)
{
    string joinS1 = "";
    string joinS2 = "";
    FOR(i, 0, word1.size())
    {
        joinS1 += word1[i];
    }
    FOR(i, 0, word2.size())
    {
        joinS2 += word2[i];
    }
    return joinS1 == joinS2;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<string> a = {"ab", "c"};
    vector<string> b = {"a", "bc"};
    cout << arrayStringsAreEqual(a, b);

    return 0;
}
