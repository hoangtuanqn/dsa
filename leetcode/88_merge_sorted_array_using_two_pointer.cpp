// 27. Remove Element
// Sử dụng 2 con trỏ
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
void merge(vector<int> &n1, int n, vector<int> &n2, int m)
{
    int i = 0, j = 0, k = 0;
    int n3[n + m];
    while (i < n && j < m && k < m + n)
    {
        if (n1[i] <= n2[j])
        {
            n3[k] = n1[i];
            i++;
        }
        else
        {
            n3[k] = n2[j];
            j++;
        }
        ++k;
    }
    for (; i < n; ++i)
    {
        n3[k] = n1[i];

        ++k;
    }
    for (; j < m; ++j)
    {
        n3[k] = n2[j];
        ++k;
    }
    // n1 = move(n3);
    FOR(i, 0, k)
    {
        n1[i] = n3[i];
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
