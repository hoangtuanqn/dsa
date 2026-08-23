#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int threeSumClosest(vector<int> &nums, int target)
{
    int n = nums.size();
    int ans = nums[0] + nums[1] + nums[2];
    int minAns = INT_MAX;
    sort(nums.begin(), nums.end());
    FOR(i, 0, n - 2)
    {
        int l = i + 1, r = n - 1;
        while (l < r)
        {
            int total = nums[i] + nums[l] + nums[r];
            if (total == target)
                return target;
            else if (total > target)
                r--;
            else
                l++;

            if (abs(total - target) < minAns)
            {
                minAns = abs(total - target);
                ans = total;
            }
        }
    }
    return ans;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> arr = {4, 0, 5, -5, 3, 3, 0, -4, -5};
    cout << threeSumClosest(arr, -2);

    return 0;
}
