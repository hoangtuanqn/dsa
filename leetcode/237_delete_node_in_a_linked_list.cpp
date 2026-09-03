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
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {
    }
};
void deleteNode(ListNode* node) {
    node->val = node->next->val;
    node->next = node->next->next;
}
void print(ListNode* head) {
    while (head) {
        cout << head->val;
        if (head->next)
            cout << " -> ";
        head = head->next;
    }
    cout << endl;
}
// ListNode* build(vector<int> vals) {
//     ListNode dummy;
//     ListNode* cur = &dummy;
//     for (int v : vals) {
//         cur->next = new ListNode(v);
//         cur = cur->next;
//     }
//     return dummy.next;
// }
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    ListNode* n1 = new ListNode(4);
    ListNode* n2 = new ListNode(5);
    ListNode* n3 = new ListNode(1);
    ListNode* n4 = new ListNode(9);
    n1->next = n2;
    n2->next = n3;
    n3->next = n4;

    print(n1);  // before: 4 -> 5 -> 1 -> 9

    deleteNode(n2);  // xóa node val=5

    print(n1);  // expected: 4 -> 1 -> 9
    return 0;
}
