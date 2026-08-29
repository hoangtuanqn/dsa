#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
string truncateSentence(string s, int k) {
    int count = 0;
    int i = 0;
    for (; i < s.size(); ++i) {
        if (s[i] == ' ')
            ++count;
        if (count == k)
            break;
    }
    return s.substr(0, i);
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s = "anh yeu em";
    int k = 2;
    cout << truncateSentence(s, k);

    return 0;
}
