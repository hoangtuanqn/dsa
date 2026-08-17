// 1. Two Sum
#include <bits/stdc++.h>
using namespace std;
vector<int> twoSum(vector<int> &nums, int target)
{
    unordered_map<int, int> maps;
    int count = nums.size();
    for (int i = 0; i < count; ++i)
    {
        int complement = target - nums[i];
        auto it = maps.find(complement);
        if (it != maps.end())
            return {maps.at(complement), i};
        maps.insert({nums[i], i});
    }
    return {};
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int n, target;
    cin >> n >> target;
    vector<int> nums(n);
    for (int i = 0; i < n; ++i)
        cin >> nums[i];

    auto ok = twoSum(nums, target);
    cout << ok[0] << " and " << ok[1];
    return 0;
}
