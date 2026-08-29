#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
    vector<int> nums2(nums.begin(), nums.end());
    sort(nums2.begin(), nums2.end());
    for (int i = 0; i < nums.size(); ++i) {
        int v = nums[i];
        auto it = lower_bound(nums2.begin(), nums2.end(), v);
        nums[i] = it - nums2.begin();
    }
    return nums;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {8, 1, 2, 2, 3};
    for (auto x : smallerNumbersThanCurrent(arr)) {
        cout << x << " ";
    }

    return 0;
}
