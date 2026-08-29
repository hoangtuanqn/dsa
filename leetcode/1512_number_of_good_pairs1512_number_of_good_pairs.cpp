#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int sumN(int n) {
    return n * (n + 1) / 2;
}
int numIdenticalPairs(vector<int>& nums) {
    unordered_map<int, int> freq;
    for (int x : nums)
        freq[x]++;
    int ans = 0;
    for (auto x : freq)
        ans += sumN(x.second - 1);
    return ans;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {1, 2, 3};
    cout << numIdenticalPairs(arr);

    return 0;
}
