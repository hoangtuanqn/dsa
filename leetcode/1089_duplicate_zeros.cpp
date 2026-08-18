// 1089. Duplicate Zeros
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
void duplicateZeros(vector<int> &arr)
{
    vector<int> arr2(arr.size());
    int j = 0;
    FOR(i, 0, arr.size())
    {
        if (j >= arr.size())
            break;
        if (arr[i] == 0)
        {
            arr2[j] = 0;
            if (j >= arr.size() - 1)
                break;
            arr2[j + 1] = 0;
            j += 2;
        }
        else
        {
            arr2[j++] = arr[i];
        }
    }
    arr = arr2;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> a = {1, 0, 2, 3, 0, 4, 5};
    duplicateZeros(a);

    return 0;
}
