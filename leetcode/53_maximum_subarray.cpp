#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int maxSubArray(vector<int> &nums)
{
    int ans = nums[0], n = nums.size(), total = 0;
    FOR(i, 0, n)
    {
        total += nums[i];
        ans = max(ans, total);
        if (total < 0)
            total = 0;
    }
    return ans;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {5, 4, -1, 7, 8};
    cout << maxSubArray(arr);

    return 0;
}
