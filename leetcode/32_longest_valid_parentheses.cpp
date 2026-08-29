#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int longestValidParentheses(string s) {
    stack<int> stack;
    int ans = 0;
    stack.push(-1);
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '(') {
            stack.push(i);
        } else {
            stack.pop();
            if (stack.empty()) {
                stack.push(i);
            } else {
                ans = max(ans, i - stack.top());
            }
        }
    }
    return ans;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s;
    cin >> s;
    // string s = "()()";
    // string s = "(())";
    // string s = "()(()";
    // string s = ")()())";
    // cin >> s;
    cout << longestValidParentheses(s);
    return 0;
}
