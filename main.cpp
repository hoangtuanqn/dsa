#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define vi vector<int>
#define ll long long
#define vii vector<pair<int, int>>
void sortColors(vi& arr) {
    int n = arr.size();
    int l = 0, m = 0, h = n - 1;
    while (m <= h) {
        if (arr[m] == 0) {
            swap(arr[l], arr[m]);
            m++;
            l++;
        } else if (arr[m] == 1) {
            m++;
        } else {
            swap(arr[h], arr[m]);
            h--;
        }
    }
}
sortColors
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vi arr = {1, 0, 2, 2, 1, 1, 0, 0, 0};
    dutch_national_flag(arr);
    for (auto& x : arr)
        cout << x << " ";

    return 0;
}
