// 977. Squares of a Sorted Array
// Sử dụng 2 con trỏ
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
vector<int> sortedSquares(vector<int> &nums)
{
    for (auto &num : nums)
    {
        num *= num;
    }
    sort(nums.begin(), nums.end());
    return nums;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> a = {-4, -1, 0, 3, 10};
    sortedSquares(a);
    for (auto v : a)
    {
        cout << v << " ";
    }

    return 0;
}
