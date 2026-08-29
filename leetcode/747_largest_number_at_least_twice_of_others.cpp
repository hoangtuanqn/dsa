#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int dominantIndex(vector<int>& nums) {
    int n = nums.size();
    int maxEle = nums[0], idx = 0;
    FOR(i, 1, n) {
        if (nums[i] > maxEle) {
            maxEle = nums[i];
            idx = i;
        }
    }

    FOR(i, 0, n) {
        if (i != idx && maxEle < nums[i] * 2) {
            return -1;
        }
    }
    return idx;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {3, 6, 1, 0};
    cout << dominantIndex(arr);
    return 0;
}
