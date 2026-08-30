#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define vi vector<int>
#define ll long long
#define vii vector<pair<int, int>>
int three_way_partition(vi& arr, int l, int r, int& lt, int& gt) {
    int m = l + (r - l) / 2;
    if (arr[l] > arr[m])
        swap(arr[l], arr[m]);
    if (arr[l] > arr[r])
        swap(arr[l], arr[r]);
    if (arr[m] > arr[r])
        swap(arr[m], arr[r]);
    int pivot = arr[m];
    int i = l;
    lt = l;
    gt = r;
    while (i <= gt) {
        if (arr[i] < pivot) {
            swap(arr[lt], arr[i]);
            ++lt;
            ++i;
        } else if (arr[i] > pivot) {
            swap(arr[gt], arr[i]);
            --gt;
        } else {
            ++i;
        }
    }
    return INT_MAX;
}
int quickselect(vi& arr, int l, int r, int k) {
    if (l == r)
        return arr[l];
    int lt, gt;
    three_way_partition(arr, l, r, lt, gt);
    if (k < lt) {
        return quickselect(arr, l, lt - 1, k);
    } else if (k > gt) {
        return quickselect(arr, gt + 1, r, k);
    } else {
        return arr[gt];
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
    int k = 4;  // ĐA: 2
    cout << findKthLargest(arr, k);
    return 0;
}
