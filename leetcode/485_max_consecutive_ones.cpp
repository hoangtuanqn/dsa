// 485. Max Consecutive Ones
// Sử dụng 2 con trỏ
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
int findMaxConsecutiveOnes(vector<int> &nums)
{
    int rs = 0, count = 0;
    for (auto num : nums)
    {
        if (num == 1)
            count++;
        else
        {
            rs = max(rs, count);
            count = 0;
        }
    }
    rs = max(rs, count);
    return rs;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> a = {1, 0, 1, 1, 0, 1};
    cout << findMaxConsecutiveOnes(a);

    return 0;
}
