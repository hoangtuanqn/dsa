// 27. Remove Element
// Sử dụng 2 con trỏ
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;

int removeElement(vector<int> &nums, int val)
{
    int count = 0, k = 0;
    FOR(i, 0, nums.size())
    {
        if (nums[i] != val)
        {
            nums[k] = nums[i];
            k++;
        }
        else
        {
            count++;
        }
    }
    return k;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> a = {3, 2, 2, 3};
    removeElement(a, 3);

    return 0;
}
