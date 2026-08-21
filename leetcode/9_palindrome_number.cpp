#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

// đang là số mà chuyển  thành string
bool isPalindrome(int x)
{
    string s = to_string(x);
    int n = s.size();
    FOR(i, 0, n / 2)
    {
        if (s[i] != s[n - i - 1])
        {
            return false;
        }
    }
    return true;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    // vector<int> arr = {4, 5, 32, 1, 2, 3, 9, 2, -3, 4};
    // sort(arr.begin(), arr.end());
    // cout << search(arr, 32);
    cout << isPalindrome(879978);

    return 0;
}
