#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

int findMiddleIndex(vector<int>& nums) {
    int n = nums.size();
    vector<int> prefix(n + 1, 0);
    prefix[0] = 0;
    FOR(i, 1, n + 1) {
        prefix[i] = prefix[i - 1] + nums[i - 1];
    }
    FOR(i, 1, n + 1) {
        if (prefix[n] - prefix[i] == prefix[i - 1])
            return i - 1;
    }
    return -1;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {2, 5};
    cout << findMiddleIndex(arr);
    return 0;
}
