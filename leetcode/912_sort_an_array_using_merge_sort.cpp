#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

// sử dụng thuật toán merge sort
void merge(vector<int> &arr, int l, int m, int r)
{
    int left_end = m - l, right_end = r - l, k = l;
    vector<int> tmp(arr.begin() + l, arr.begin() + r);
    int i = 0, j = m - l;
    while (i < left_end && j < right_end)
    {
        if (tmp[i] <= tmp[j])
            arr[k++] = tmp[i++];
        else
            arr[k++] = tmp[j++];
    }
    while (i < left_end)
        arr[k++] = tmp[i++];
    while (j < right_end)
        arr[k++] = tmp[j++];
}
void merge_sort(vector<int> &arr, int l, int r)
{
    if (r - l <= 1)
        return;
    int m = l + (r - l) / 2;
    merge_sort(arr, l, m);
    merge_sort(arr, m, r);
    merge(arr, l, m, r);
}
vector<int> sortArray(vector<int> &nums)
{
    merge_sort(nums, 0, nums.size());
    return nums;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {4, 3};
    sortArray(arr);
    for (auto v : arr)
    {
        cout << v << " ";
    }
    return 0;
}
