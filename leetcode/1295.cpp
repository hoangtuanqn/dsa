// 1295. Find Numbers with Even Number of Digits
#include <bits/stdc++.h>
using namespace std;
int countNumber(int num)
{
    int count = 0;
    while (num)
    {
        num /= 10;
        count++;
    }
    return count;
}
int findNumbers(vector<int> &nums)
{
    int rs = 0;
    for (auto num : nums)
    {
        if (countNumber(num) % 2 == 0)
        {
            ++rs;
        }
    }
    return rs;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> nums = {555, 901, 482, 1771, 3421};
    cout << findNumbers(nums);

    return 0;
}
