#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

// đang là số mà chuyển  thành string
vector<int> twoSum(vector<int> &numbers, int target)
{
    int i = 0, j = numbers.size() - 1;
    while (i < j)
    {
        int total = numbers[i] + numbers[j];
        if (total == target)
        {
            return {i + 1, j + 1};
        }
        else if (total > target)
        {
            --j;
        }
        else
        {
            ++i;
        }
    }
    return {0, 0};
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    // vector<int> arr = {4, 5, 32, 1, 2, 3, 9, 2, -3, 4};
    // sort(arr.begin(), arr.end());
    // cout << search(arr, 32);
    vector<int> arr = {2, 7, 11, 15};
    auto v = twoSum(arr, 9);
    cout << v[0] << " " << v[1];

    return 0;
}
