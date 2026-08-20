#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
void tower_of_hanoi(int n, char a, char b, char c)
{
    if (n == 1)
    {
        cout << a << " " << c << endl;
    }
    else
    {
        tower_of_hanoi(n - 1, a, c, b);
        tower_of_hanoi(1, a, b, c);
        tower_of_hanoi(n - 1, b, a, c);
    }
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n;
    cin >> n;
    cout << (pow(2, n) - 1) << endl;
    tower_of_hanoi(n, '1', '2', '3');
    return 0;
}
