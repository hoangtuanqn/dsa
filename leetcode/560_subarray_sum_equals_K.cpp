#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    freq[0] = 1;
    int count = 0, prefixSum = 0;
    for (int num : nums) {
        prefixSum += num;
        count += freq[prefixSum - k];
        freq[prefixSum]++;
    }
    return count;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {1, 2, 3};
    int k = 3;
    cout << subarraySum(arr, k);
    return 0;
}
