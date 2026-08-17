// 88. Merge Sorted Array
#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
void insertElement(vector<int> &n1, int &n, int value, int pos)
{
    for (int i = n; i > pos; --i)
    {
        n1[i] = n1[i - 1];
    }
    n1[pos] = value;
    ++n;
}
int findIndexInsert(vector<int> &n1, int n, int value)
{
    FOR(i, 0, n)
    {
        if (n1[i] > value)
            return i;
    }
    return n;
}
void merge(vector<int> &n1, int n, vector<int> &n2, int m)
{
    FOR(i, 0, m)
    {
        int k = findIndexInsert(n1, n, n2[i]);
        insertElement(n1, n, n2[i], k);
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
    for (auto val : a)
    {
        cout << val << " ";
    }

    return 0;
}
