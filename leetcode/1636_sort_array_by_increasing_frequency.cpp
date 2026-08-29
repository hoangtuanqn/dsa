#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
vector<int> frequencySort(vector<int>& nums) {
    unordered_map<int, int> freq;
    for (int x : nums)
        freq[x]++;
    sort(nums.begin(), nums.end(), [&freq](int a, int b) {
        if (freq[a] == freq[b]) {
            return a > b;
        }
        return freq[a] < freq[b];
    });
    return nums;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {-1, 1, -6, 4, 5, -6, 1, 4, 1};
    frequencySort(arr);
    for (auto v : frequencySort(arr)) {
        cout << v << " ";
    }

    return 0;
}
