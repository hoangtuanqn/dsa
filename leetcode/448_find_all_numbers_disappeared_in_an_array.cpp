#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

vector<int> findDisappearedNumbers(vector<int>& nums) {
    vector<int> rs;
    // Pha 1 : gặp giá trị v → đánh dấu ô(v - 1) bằng cách bôi âm for (int x : nums) {
    for (int x : nums) {
        int idx = abs(x) - 1;
        if (nums[idx] > 0)
            nums[idx] *= -1;
    }
    // Pha 2: ô nào còn dương → index đó chưa ai đánh dấu → thiếu
    for (int i = 0; i < nums.size(); ++i)
        if (nums[i] > 0)
            rs.push_back(i + 1);
    return rs;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {4, 3, 2, 7, 8, 2, 3, 1};
    for (auto v : findDisappearedNumbers(arr)) {
        cout << v << " ";
    }
    return 0;
}
