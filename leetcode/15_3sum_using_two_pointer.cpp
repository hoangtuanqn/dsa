#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
vector<vector<int>> threeSum(vector<int> &nums)
{
    vector<vector<int>> rs;
    sort(nums.begin(), nums.end());
    int n = nums.size();
    FOR(i, 0, n)
    {
        if (nums[i] > 0)
            break;
        if (i > 0 && nums[i - 1] == nums[i])
            continue;
        int l = i + 1, r = n - 1;
        while (l < r)
        {
            int sum = nums[l] + nums[r] + nums[i];
            if (sum == 0)
            {
                rs.push_back({nums[l], nums[r], nums[i]});
                while (l < r && nums[l] == nums[l + 1])
                    l++;
                while (l < r && nums[r] == nums[r - 1])
                    r--;

                l++;
                r--;
            }
            else if (sum > 0)
                r--;
            else
                l++;
        }
    }
    return rs;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> arr = {-1, 0, 1, 2, -1, -4};
    vector<vector<int>> ok = threeSum(arr);
    for (auto &val : ok)
    {
        for (auto &vall : val)
        {
            cout << vall << " ";
        }
        cout << endl;
    }
    return 0;
}
