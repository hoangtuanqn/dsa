#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

// gọi dp[i] là số lớn nhất tính từ i tới n
vector<int> replaceElements(vector<int>& arr) {
    int mx = -1;
    for (int i = arr.size() - 1; i >= 0; --i) {
        int cur = arr[i];
        arr[i] = mx;
        if (cur > mx)
            mx = cur;
    }
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
