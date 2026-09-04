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
vector<int> sortEvenOdd(vector<int>& nums) {
    int n = nums.size();
    for (int i = 0; i < n / 2; ++i) {
        for (int j = 1; j < n - i - 2; j += 2) {
            if (nums[j] < nums[j + 2]) {
                swap(nums[j], nums[j + 2]);
            }
        }
    }
    for (int i = 0; i < n / 2; ++i) {
        for (int j = 0; j < n - i - 2; j += 2) {
            if (nums[j] > nums[j + 2]) {
                swap(nums[j], nums[j + 2]);
            }
        }
    }
    return nums;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {1, 2, 3, 4};  // 1 4 3 2
    arr = sortEvenOdd(arr);
    each(x, arr) {
        cout << x << " ";
    }
    return 0;
}
