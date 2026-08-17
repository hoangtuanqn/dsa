#include <bits/stdc++.h>
using namespace std;
void insertElement(int ai, int pos, vector<int> &nums1, int &n)
{
    for (int i = n; i > pos; --i)
    {
        nums1[i] = nums1[i - 1];
    }
    nums1[pos] = ai;
    n++;
}
int findIndexCanInsert(vector<int> &n1, int n, int n2i)
{
    bool isFind = false;
    for (int i = 0; i < n; ++i)
    {
        if (n1[i] > n2i)
            return i;
    }
    return n;
}
void merge(vector<int> &n1, int n, vector<int> &n2, int m)
{
    for (int i = 0; i < m; ++i)
    {
        int k = findIndexCanInsert(n1, n, n2[i]);
        insertElement(n2[i], k, n1, n);
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<int> a = {1, 2, 3, 0, 0, 0};
    vector<int> b = {2, 3, 3};
    merge(a, 3, b, 3);
    for (auto val : a)
    {
        cout << val << " ";
    }

    return 0;
}
