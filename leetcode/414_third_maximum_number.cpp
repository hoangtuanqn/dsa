#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
int thirdMax(vector<int> &nums)
{
    sort(nums.begin(), nums.end(), greater<int>());
    int maxCount = 1;
    int valMax = nums[0];
    FOR(i, 1, nums.size())
    {
        if (nums[i] != valMax)
        {
            maxCount++;
            valMax = nums[i];
            if (maxCount == 3)
                return nums[i];
        }
    }
    return nums[0];
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> arr = {10, 5, 2, 9, -20, 2, 3}; // -33 0 2 5 9 23
    cout << thirdMax(arr);
    return 0;
}
