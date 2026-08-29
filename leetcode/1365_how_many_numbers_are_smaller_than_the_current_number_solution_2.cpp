#include <bits/stdc++.h>
#include <algorithm>
#include <unordered_map>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
// gọi freq[i] là số lượng số nhỏ hơn i + 1. Ví dụ freq[5]= 20 là có 20 số nhỏ hơn 6.
vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
    int freq[102] = {0};
    vector<int> ans;
    ans.reserve(nums.size());
    cout << ans.size() << endl;
    for (int x : nums)
        freq[x]++;
    for (int i = 1; i <= 101; ++i) {
        freq[i] += freq[i - 1];
    }
    for (int x : nums) {
        ans.push_back(x == 0 ? 0 : freq[x - 1]);
    }
    return ans;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {8, 1, 2, 2, 3};
    for (auto x : smallerNumbersThanCurrent(arr)) {
        cout << x << " ";
    }

    return 0;
}
