#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int myAtoi(string s)
{
    int n = s.size();
    int i = 0;
    while (s[i] == ' ')
        ++i;
    int sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '-' || s[i] == '+')
    {
        ++i;
    }
    int ans = 0;
    while (isdigit(s[i]))
    {
        int digit = s[i] - '0';
        if (ans > (INT_MAX - digit) / 10)
        {
            return sign == -1 ? INT_MIN : INT_MAX;
        }
        ans = ans * 10 + digit;

        ++i;
    }
    return ans * sign;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<string> arr = {""};
    cout << myAtoi("-2147483648") << endl;
    cout << myAtoi("-2147483649") << endl;
    // cout << myAtoi(" -042") << endl;
    // cout << myAtoi("1337c0d3") << endl;
    // cout << myAtoi("0-1") << endl;
    // cout << myAtoi("words and 987") << endl;
    // cout << myAtoi("-91283472332") << endl;
    // cout << myAtoi("-2147483648") << endl;

    return 0;
}
