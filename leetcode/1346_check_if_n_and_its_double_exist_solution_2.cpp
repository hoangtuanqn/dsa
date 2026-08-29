#include <bits/stdc++.h>
#include <algorithm>
#include <bitset>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
bool checkIfExist(vector<int>& arr) {
    int n = arr.size();
    constexpr int OFFSET = 1000;
    bitset<2001> seen;
    for (auto x : arr) {
        if (x % 2 == 0 && seen[x / 2 + OFFSET])
            return true;

        int t = 2 * x + OFFSET;
        if (t >= 0 && t <= 2000 && seen[t])
            return true;
        seen[x + OFFSET] = 1;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {10, 2, 5, 3};
    cout << checkIfExist(arr);
    return 0;
}
