#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
int removeDuplicates(vector<int>& nums) {
    int k = 0, n = nums.size();
    FOR(i, 1, n) {
        if (nums[k] != nums[i]) {
            k++;
            nums[k] = nums[i];
        }
    }

    return k + 1;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    cout << removeDuplicates(arr);
    return 0;
}
