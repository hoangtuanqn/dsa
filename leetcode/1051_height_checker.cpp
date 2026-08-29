#include <bits/stdc++.h>
using namespace std;
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define ll long long
#define vii vector<int, int>

int heightChecker(vector<int>& heights) {
    int arr[101] = {0}, ans = 0;
    for (int i = 0; i < heights.size(); ++i) {
        arr[heights[i]]++;
    }
    int k = 0;
    for (int i = 0; i < heights.size(); ++i) {
        while (arr[k] == 0)
            k++;
        if (heights[i] != k) {
            ans++;
        }
        arr[k]--;
    }
    return ans;
}
int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    vector<int> arr = {5, 1, 2, 3, 4};
    cout << heightChecker(arr);
    return 0;
}
