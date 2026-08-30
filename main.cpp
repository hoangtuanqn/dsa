#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define vi vector<int>
#define ll long long
#define vii vector<pair<int, int>>
int median_of_three(vi& nums, int l, int r) {
    int m = l + (r - l) / 2;
    if (nums[l] > nums[m])
        swap(nums[l], nums[m]);
    if (nums[l] > nums[r])
        swap(nums[l], nums[r]);
    if (nums[m] > nums[r])
        swap(nums[m], nums[r]);
    return nums[m];
}
void three_way_partition(vi& nums, int l, int r, int& lt, int& gt) {
    int pivot = median_of_three(nums, l, r);
    int i = l;
    lt = l;
    gt = r;
    while (i <= gt) {
        if (nums[i] < pivot) {
            swap(nums[lt], nums[i]);
            ++i;
            ++lt;
        } else if (nums[i] > pivot) {
            swap(nums[gt], nums[i]);
            --gt;
        } else {
            ++i;
        }
    }
}
int quickselect(vi& nums, int l, int r, int k) {
    if (l == r)
        return nums[l];
    int lt, gt;
    three_way_partition(nums, l, r, lt, gt);
    if (k < lt) {
        return quickselect(nums, l, lt - 1, k);
    } else if (k > gt) {
        return quickselect(nums, gt + 1, r, k);
    } else {
        return nums[lt];
    }
}
int findKthLargest(vector<int>& nums, int k) {
    int n = nums.size();
    return quickselect(nums, 0, n - 1, n - k);
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vi arr = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    int k = 4;
    cout << findKthLargest(arr, k);  // 4;
    return 0;
}
