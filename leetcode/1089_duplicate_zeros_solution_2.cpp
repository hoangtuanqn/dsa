// 977. Squares of a Sorted Array
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
void duplicateZeros(vector<int> &arr)
{
    int write_pos = 0, total = 0, pos = 0;
    bool last_zero_truncated = false;
    FOR(i, 0, arr.size())
    {
        total += arr[i] == 0 ? 2 : 1;
        if (total >= arr.size())
        {
            last_zero_truncated = total > arr.size();
            pos = i;
            break;
        }
    }
    int j = arr.size() - 1;
    for (int i = pos; i >= 0; --i)
    {
        if (arr[i] == 0)
        {
            arr[j] = 0;
            if (!(last_zero_truncated && i == pos))
            {
                j--;
                arr[j] = 0;
            }
        }
        else
            arr[j] = arr[i];
        j--;
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    // vector<int> a = {1, 0, 0, 2};             // 1 0 0 2 3 0 0
    vector<int> a = {1, 0, 2, 3, 0, 4, 5, 0}; // [1,0,0,2,3,0,0,4]

    duplicateZeros(a);

    return 0;
}
