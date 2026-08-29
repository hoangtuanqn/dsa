#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

bool validMountainArray(vector<int>& arr) {
    int i = 0, n = arr.size();
    while (i < n - 1 && arr[i] < arr[i + 1])
        i++;
    if (i == 0 || i == n - 1)
        return false;
    while (i < n - 1 && arr[i] > arr[i + 1])
        i++;
    return i == n - 1;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {5, 7, 3};
    cout << validMountainArray(arr);
    return 0;
}
