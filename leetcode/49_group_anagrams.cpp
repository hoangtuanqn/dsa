#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")
#include <bits/stdc++.h>
using namespace std;

// Types
#define ll long long
#define ull unsigned long long
#define ld long double
#define pii pair<int, int>
#define pll pair<ll, ll>

// Vectors
#define vi vector<int>
#define vl vector<ll>
#define vii vector<pii>
#define vb vector<bool>

// Loops
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define RFOR(i, a, b) for (int i = (a); i >= (b); --i)
#define each(x, v) for (auto& x : v)

// Shortcuts
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define sz(x) (int)(x).size()
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

// Utils
#define YES cout << "YES\n"
#define NO cout << "NO\n"
#define dbg(x) cerr << #x << " = " << x << "\n"

// Constants
const int INF = 1e9 + 7;
const ll LINF = 1e18;
const int MOD = 1e9 + 7;

// ====== SOLUTION
string freq_string(string s) {
    array<int, 26> freq{};
    for (int c : s) {
        freq[c - 'a']++;
    }
    string key;
    key.reserve(26 * 2);
    for (int& x : freq) {
        key += x;
        key += '#';
    }
    return key;
}
vector<vector<string>> groupAnagrams(vector<string>& strs) {
    vector<vector<string>> ans;
    unordered_map<string, vector<string>> map;
    FOR(i, 0, strs.size()) {
        string temp = strs[i];
        map[freq_string(strs[i])].push_back(strs[i]);
    }
    for (auto& v : map) {
        ans.push_back(move(v.second));
    }
    return ans;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    return 0;
}
