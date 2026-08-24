#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

vector<vector<int>> fourSum(vector<int> &nums, int target)
{
    int n = nums.size();
    vector<vector<int>> results;
    if (n < 4)
        return results;
    sort(nums.begin(), nums.end());
    for (int i = 0; i <= n - 4; ++i)
    {
        if (i > 0 && nums[i] == nums[i - 1])
            continue;
        // Pruning: bộ 4 nhỏ nhất với i này đã > target
        if (1LL * nums[i] + nums[i + 1] + nums[i + 2] + nums[i + 3] > target)
            break;
        // Pruning: bộ 4 lớn nhất với i này vẫn < target
        if (1LL * nums[i] + nums[n - 3] + nums[n - 2] + nums[n - 1] < target)
            continue;
        for (int j = i + 1; j <= n - 3; ++j)
        {
            // tại sao j > i + 1?
            // Vì j chỉ dc skip khi đã chạy ít nhất 1 lần, mà j start là i + 1, nên j > i + 1 thì mới chạy dc 1 lần.
            if (j > i + 1 && nums[j] == nums[j - 1])
                continue;
            // Pruning: bộ 4 nhỏ nhất với (i, j) này đã > target
            if (1LL * nums[i] + nums[j] + nums[j + 1] + nums[j + 2] > target)
                break;
            // Pruning: bộ 4 lớn nhất với (i, j) này vẫn < target
            if (1LL * nums[i] + nums[j] + nums[n - 1] + nums[n - 2] < target)
                continue;

            int l = j + 1, r = n - 1;
            while (l < r)
            {
                ll total = 1LL * nums[i] + nums[j] + nums[l] + nums[r];
                if (total == target)
                {
                    results.push_back({nums[i], nums[j], nums[l], nums[r]});
                    while (l < r && nums[l] == nums[l + 1])
                        l++;
                    while (l < r && nums[r] == nums[r - 1])
                        r--;
                    l++;
                    r--;
                }
                else if (total > target)
                    r--;
                else
                    l++;
            }
        }
    }
    return results;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> nums = {-2, -1, -1, 1, 1, 2, 2};
    int target = 0;
    for (auto &arr : fourSum(nums, target))
    {
        for (auto val : arr)
        {
            cout << val << " ";
        }
        cout << endl;
    }
    return 0;
}
