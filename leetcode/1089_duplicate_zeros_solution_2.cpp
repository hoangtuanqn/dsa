// 977. Squares of a Sorted Array
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
void duplicateZeros(vector<int> &arr)
{
    int usage = 0, pos = 0;
    while (usage < arr.size())
    {
        usage += arr[pos++] == 0 ? 2 : 1;
    }
    bool last_zero_truncated = (usage - 1) == arr.size();
    pos--;
    int j = arr.size() - 1;
    if (last_zero_truncated)
    {
        arr[j--] = 0;
        pos--;
    }
    for (int i = pos; i >= 0; --i)
    {
        if (arr[i] == 0)
        {
            arr[j] = arr[j - 1] = 0;
            j -= 2;
        }
        else
        {
            arr[j--] = arr[i];
        }
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
