// 27. Remove Element
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
void remove(vector<int> &nums, int &n, int pos)
{
    FOR(i, pos, n - 1)
    {
        nums[i] = nums[i + 1];
    }
    n--;
}
int removeElement(vector<int> &nums, int val)
{
    int count = 0;
    int n = nums.size();
    for (int i = n - 1; i >= 0; --i)
    {
        if (nums[i] == val)
        {
            remove(nums, n, i);
            count++;
        }
    }
    FOR(i, 0, n)
    {
        cout << nums[i] << " ";
    }
    return nums.size() - count;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> a = {2, 2, 5, 4, 2, 2, 2, 22, 5, 6, 2};
    removeElement(a, 2);

    return 0;
}
