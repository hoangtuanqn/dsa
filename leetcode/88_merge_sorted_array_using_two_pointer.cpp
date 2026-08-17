// 27. Remove Element
// Sử dụng 2 con trỏ
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
void merge(vector<int> &n1, int n, vector<int> &n2, int m)
{
    int i = n - 1, j = m - 1, k = m + n - 1;
    while (k >= 0)
    {
        if (j < 0)
        {
            n1[k] = n1[i];
            i--;
        }
        else if (i < 0)
        {
            n1[k] = n2[j];
            j--;
        }
        else if (n1[i] > n2[j])
        {
            n1[k] = n1[i];
            i--;
        }
        else
        {
            n1[k] = n2[j];
            --j;
        }
        k--;
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> a = {1, 2, 3, 0, 0, 0};
    vector<int> b = {2, 5, 6};
    merge(a, 3, b, 3);

    return 0;
}
