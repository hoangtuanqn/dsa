#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
// idea: Fast & Slow Pointer
void moveZeroes(vector<int> &nums)
{
    int k = 0, n = nums.size();
    FOR(i, 0, n)
    {
        if (nums[i] != 0)
        {
            nums[k] = nums[i];
            ++k;
        }
    }
    FOR(i, k, n) {
        nums[i] = 0;
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    // vector<int> arr = {0, 1, 0, 3, 12};
    vector<int> arr = {4, 0, 5};

    moveZeroes(arr);
    for (auto v : arr)
    {
        cout << v << " ";
    }
    return 0;
}
