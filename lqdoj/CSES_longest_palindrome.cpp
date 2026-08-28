#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
string init(string s) {
    string result = "^#";
    FOR(i, 0, s.size()) {
        result += s[i];
        result += "#";
    }
    result += "$";
    return result;
}
vector<int> manacher(string s) {
    string t = init(s);
    int n = t.size();
    vector<int> p(n, 0);
    int center = 0, rightEdge = 0;
    for (int i = 1; i < n - 1; ++i) {
        int mirror = 2 * center - i;
        if (i < rightEdge) {
            p[i] = min(rightEdge - i, p[mirror]);
        }
        while (t[i - p[i] - 1] == t[i + p[i] + 1]) {
            p[i]++;
        }
        if (i + p[i] > rightEdge) {
            rightEdge = i + p[i];
            center = i;
        }
    }
    return p;
}
string resolve(string s) {
    vector<int> p = manacher(s);
    int centerBest = 0, maxLength = 0;
    for (int i = 0; i < p.size(); ++i) {
        if (p[i] > maxLength) {
            maxLength = p[i];
            centerBest = i;
        }
    }
    int start = (centerBest - maxLength) / 2;
    return s.substr(start, maxLength);
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s;
    cin >> s;
    cout << resolve(s);
    return 0;
}
