#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
// using DNF (Dutch National Flag)
void sortColors(vector<int> &nums)
{
    int l = 0, m = 0, h = nums.size() - 1;
    while (m <= h)
    {
        if (nums[m] == 0)
        {
            swap(nums[l], nums[m]);
            m++;
            l++;
        }
        else if (nums[m] == 1)
            m++;
        else if (nums[m] == 2)
        {
            swap(nums[h], nums[m]);
            h--;
        }
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    // vector<int> arr = {0, 1, 0, 3, 12};
    vector<int> arr = {2, 0, 2, 1, 1, 0};

    sortColors(arr);
    for (auto v : arr)
    {
        cout << v << " ";
    }
    return 0;
}
