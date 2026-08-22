#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

void bubble_sort(vector<int> &arr)
{
    int n = arr.size();
    for (int i = 0; i < n; ++i)
    {
        bool isSwap = false;
        for (int j = 0; j < n - 1 - i; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                isSwap = true;
                swap(arr[j], arr[j + 1]);
            }
        }
        if (!isSwap)
            break;
    }
}
void sortColors(vector<int> &nums)
{
    bubble_sort(nums);
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {-44, 2, 1, 4, 5, 6, -33, 7};
    bubble_sort(arr);
    for (auto v : arr)
    {
        cout << v << " ";
    }
    return 0;
}
