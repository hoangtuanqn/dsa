#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

vector<int> sortArrayByParity(vector<int> &nums)
{
    int i = 0, j = nums.size() - 1;
    while (i < j)
    {
        if (nums[i] % 2 == 0)
        {
            i++;
        }
        else
        {
            swap(nums[i], nums[j]);
            j--;
        }
    }
    return nums;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {3, 1, 2, 4};
    sortArrayByParity(arr);
    for (auto v : arr)
    {
        cout << v << " ";
    }
    return 0;
}
