#include <algorithm>
#pragma GCC optimize("O2")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")
#include <bits/stdc++.h>
using namespace std;

// Types
#define ll long long
#define ull unsigned long long
#define ld long double
#define pii pair<int, int>
#define pll pair<ll, ll>

// Vectors
#define vi vector<int>
#define vl vector<ll>
#define vii vector<pii>
#define vb vector<bool>

// Loops
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define RFOR(i, a, b) for (int i = (a); i >= (b); --i)
#define each(x, v) for (auto& x : v)

// Shortcuts
#define pb push_back
#define mp make_pair
#define fi first
#define se second
#define sz(x) (int)(x).size()
#define all(x) (x).begin(), (x).end()
#define rall(x) (x).rbegin(), (x).rend()

// Utils
#define YES cout << "YES\n"
#define NO cout << "NO\n"
#define dbg(x) cerr << #x << " = " << x << "\n"

// Constants
const int INF = 1e9 + 7;
const ll LINF = 1e18;
const int MOD = 1e9 + 7;

// ====== SOLUTION

// reverse trick (đảo ngược mảng)
// b1: Đảo ngược all mảng
// b2: đảo ngược [0..k-1]
// b3: đảo ngược [k..n-1]4

// void reverse(vector<int>& nums, int l, int r) {
//     if(r < l) return;
//     int n = (r - l) / 2;
//     for (int i = 0; i <= n; ++i) {
//         swap(nums[l + i], nums[r - i]);
//     }
// }
void rotate(vector<int>& nums, int k) {
    int n = nums.size();
    k %= n;
    reverse(nums.begin(), nums.end());
    reverse(nums.begin(), nums.begin() + k);
    reverse(nums.begin() + k, nums.end());
    // reverse(nums, 0, n - 1);
    // reverse(nums, 0, k - 1);
    // reverse(nums, k, n - 1);
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7};

    int k = 3;
    rotate(arr, k);
    for (auto x : arr) {
        cout << x << " ";
    }

    return 0;
}
