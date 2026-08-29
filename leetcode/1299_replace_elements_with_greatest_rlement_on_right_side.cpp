#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

// gọi dp[i] là số lớn nhất tính từ i tới n
vector<int> replaceElements(vector<int>& arr) {
    int n = arr.size();
    vector<int> dp(n, 0);
    int numMax = 0;
    for (int i = n - 1; i >= 0; --i) {
        dp[i] = max(numMax, arr[i]);
        numMax = max(numMax, arr[i]);
    }

    for (int i = 0; i < n - 1; ++i) {
        arr[i] = dp[i + 1];
    }
    arr[n - 1] = -1;
    return arr;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {3, 9, 6, 3, 1, 3};
    replaceElements(arr);
    return 0;
}
