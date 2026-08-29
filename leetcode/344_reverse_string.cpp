#include <bits/stdc++.h>
#include <climits>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
void reverseString(vector<char>& s) {
    int n = s.size();
    for (int i = 0; i < n / 2; ++i) {
        swap(s[i], s[n - i - 1]);
    }
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<char> arr = {'h', 'e', 'l', 'l', 'o'};
    reverseString(arr);
    for (char v : arr) {
        cout << v << " ";
    }
    return 0;
}
