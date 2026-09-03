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
struct Node {
    int val;
    Node* next;
    Node(int val) : val{val}, next{nullptr} {};
};
class MyLinkedList {
   private:
    Node* tail = nullptr;
    Node* head = nullptr;
    int sz = 0;

   public:
    MyLinkedList() {
    }

    int get(int index) {
        if (index < 0 || index >= sz)
            return -1;
        Node* cur = head;
        for (int i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur->val;
    }

    void addAtHead(int val) {
        Node* n = new Node(val);
        if (head) {
            n->next = head;
        } else {
            tail = n;
        }
        head = n;
        ++sz;
    }

    void addAtTail(int val) {
        Node* n = new Node(val);
        if (tail) {
            tail->next = n;
        } else {
            head = n;
        }
        tail = n;
        ++sz;
    }

    void addAtIndex(int index, int val) {
        if (index < 0 || index > sz)
            return;
        if (index == 0) {
            addAtHead(val);
            return;
        }
        if (index == sz) {
            addAtTail(val);
            return;
        }
        Node* cur = head;
        for (int i = 0; i < index - 1 && cur; ++i) {
            cur = cur->next;
        }
        Node* n = new Node(val);
        n->next = cur->next;
        cur->next = n;
        ++sz;
    }
    void pop_front() {
        if (!head)
            return;
        Node* nxt = head->next;
        delete head;
        head = nxt;
        if (!head)
            tail = nullptr;
        --sz;
    }
    void pop_back() {
        if (!tail)
            return;

        Node* cur = head;
        while (cur->next != tail) {
            cur = cur->next;
        }

        delete tail;
        cur->next = nullptr;
        tail = cur;
        --sz;
    }
    void deleteAtIndex(int index) {
        if (index < 0 || index >= sz)
            return;
        if (index == 0) {
            pop_front();
            return;
        }
        if (index == sz - 1) {
            pop_back();
            return;
        }
        Node* prev = nullptr;
        Node* cur = head;
        for (int i = 0; i < index && cur; ++i) {
            prev = cur;
            cur = cur->next;
        }
        prev->next = cur->next;
        delete cur;
        --sz;
    }
};
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    MyLinkedList l;
    l.addAtHead(1);
    l.addAtTail(3);
    l.addAtIndex(1, 2);        // linked list becomes 1->2->3
    cout << l.get(1) << "\n";  // return 2
    l.deleteAtIndex(1);        // now the linked list is 1->3
    cout << l.get(1);          // return 3

    return 0;
}
