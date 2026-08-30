#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

int findKthLargest(vector<int>& nums, int k) {
    nth_element(nums.begin(), nums.end() - k, nums.end());
    return nums[nums.size() - k];
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {5, 1, 2, 3, 4};
    cout << findKthLargest(arr, 2);
    return 0;
}
