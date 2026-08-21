#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int reverse(int x)
{
    int ans = 0;
    while (x)
    {
        if (ans > INT_MAX / 10 || ans < INT_MIN / 10)
        {
            ans = 0;
            break;
        }
        ans = ans * 10 + x % 10;
        x /= 10;
    }
    return ans;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<string> arr = {""};
    cout << reverse(123) << endl;
    cout << reverse(-123) << endl;
    cout << reverse(120) << endl;
    cout << reverse(INT_MAX) << endl;
    cout << reverse(INT_MIN) << endl;
    cout << -15 % 10;

    // PINALSIGYAHRPI
    // PINALSIGYAHRPPII
    return 0;
}
