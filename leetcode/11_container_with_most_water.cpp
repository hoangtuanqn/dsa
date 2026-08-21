#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int maxArea(vector<int> &height)
{
    int l = 0, r = height.size() - 1;
    int result = 0;
    while (l < r)
    {
        result = max(result, min(height[l], height[r]) * (r - l));
        if (height[l] > height[r])
            r--;
        else
            l++;
    }
    return result;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> arr = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << maxArea(arr);
    return 0;
}
