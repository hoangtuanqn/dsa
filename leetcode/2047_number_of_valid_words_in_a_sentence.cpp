#include <cctype>
#include <string_view>
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
vector<string> splitSentence(string& s) {
    s += " ";
    vector<string> ans;
    size_t start = 0;
    size_t end = s.find(' ');
    while (end != string::npos) {
        if (end > start)
            ans.push_back(s.substr(start, end - start));

        start = end + 1;
        end = s.find(' ', start);
    }
    return ans;
}
bool isValidWord(string s) {
    int n = s.size();
    int hyphen = 0;
    for (int i = 0; i < n; ++i) {
        if (isdigit(s[i]))
            return false;
        if ((s[i] == '!' || s[i] == '.' || s[i] == ',') && i != n - 1)
            return false;
        if (s[i] == '-') {
            hyphen++;
            if (hyphen > 1)
                return false;
        }
        if (s[i] == '-' && ((i == 0 || i == n - 1) || (!isalpha(s[i - 1]) || !isalpha(s[i + 1]))))
            return false;
    }
    return true;
}
int countValidWords(string sentence) {
    int ans = 0;
    auto strings = splitSentence(sentence);
    for (auto s : strings) {
        ans += isValidWord(s);
    }
    return ans;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    return 0;
}
