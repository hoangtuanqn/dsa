#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int numJewelsInStones(string jewels, string stones) {
    int ans = 0;
    unordered_map<char, bool> tick;
    for (char c : jewels)
        tick[c] = true;
    for (char c : stones)
        ans += tick[c];
    return ans;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string jewels = "aA";
    string stones = "aAAbbbb";
    cout << numJewelsInStones(jewels, stones);

    return 0;
}
