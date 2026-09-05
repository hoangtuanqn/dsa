#include <climits>
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
int firstStableIndex(vector<int>& nums, int k) {
    int n = nums.size();
    vector<int> largest(n, 0);
    vector<int> smallest(n, 0);
    smallest[n - 1] = nums[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        smallest[i] = min(nums[i], smallest[i + 1]);
    }
    largest[0] = nums[0];
    if (largest[0] - smallest[0] <= k)
        return 0;
    for (int i = 1; i < n; ++i) {
        largest[i] = max(nums[i], largest[i - 1]);
        if (largest[i] - smallest[i] <= k)
            return i;
    }
    return -1;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {3, 2, 1};
    int k = 1;
    cout << firstStableIndex(arr, k);
    return 0;
}