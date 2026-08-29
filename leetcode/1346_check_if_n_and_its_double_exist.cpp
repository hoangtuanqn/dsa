#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
bool checkIfExist(vector<int>& arr) {
    int n = arr.size();
    sort(arr.begin(), arr.end());
    for (int i = 0; i < n; ++i) {
        int t = 2 * arr[i];
        auto it = lower_bound(arr.begin(), arr.end(), t);
        if (it != arr.end() && *it == t && it - arr.begin() != i) {
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {3, 1, 7, 11};
    cout << checkIfExist(arr);
    return 0;
}
