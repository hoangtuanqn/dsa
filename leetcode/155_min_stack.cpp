#include <bits/stdc++.h>
#include <climits>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>
class MinStack {
    stack<int> st;
    stack<int> minSt;

   public:
    MinStack() {
    }

    void push(int value) {
        st.push(value);
        int cur = minSt.empty() ? value : min(value, minSt.top());
        minSt.push(cur);
    }

    void pop() {
        minSt.pop();
        st.pop();
    }

    int top() {
        return st.top();
    }

    int getMin() {
        return minSt.top();
    }
};
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    MinStack* obj = new MinStack();
    obj->push(20);
    obj->push(30);
    obj->push(40);
    obj->push(10);
    // obj->pop();
    cout << obj->getMin();
    return 0;
}
