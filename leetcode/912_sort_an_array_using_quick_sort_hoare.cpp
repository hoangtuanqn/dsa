#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int median_of_three(vector<int> &arr, int l, int h)
{
    int mid = l + (h - l) / 2;
    if (arr[mid] < arr[l])
        swap(arr[mid], arr[l]);
    if (arr[l] > arr[h])
        swap(arr[l], arr[h]);
    if (arr[h] < arr[mid])
        swap(arr[h], arr[mid]);
    return arr[mid];
}
int hoare_partition(vector<int> &arr, int l, int h)
{
    int pivot = median_of_three(arr, l, h);
    int i = l - 1;
    int j = h + 1;
    while (1)
    {
        do
        {
            i++;
        } while (arr[i] < pivot);

        do
        {
            j--;
        } while (arr[j] > pivot);

        if (i >= j)
            return j;
        swap(arr[i], arr[j]);
    }
}
void quick_sort(vector<int> &arr, int l, int h)
{
    if (l < h)
    {
        int p = hoare_partition(arr, l, h);
        quick_sort(arr, l, p);
        quick_sort(arr, p + 1, h);
    }
}
vector<int> sortArray(vector<int> &nums)
{
    quick_sort(nums, 0, nums.size() - 1);
    return nums;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {6, 7, 8, 5, 4, 1, 2, 3};
    quick_sort(arr, 0, arr.size() - 1);
    for (auto v : arr)
    {
        cout << v << " ";
    }
    return 0;
}
