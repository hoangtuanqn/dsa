#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
int maxNumberOfFamilies(int n, vector<vector<int>> &reservedSeats)
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    unordered_map<int, vector<bool>> matrix;
    for (auto &s : reservedSeats)
    {
        // Có lời giải thích 1386_cinema_seat_allocation_solution_2_optimize.md
        int k = s[0], v = s[1];
        if (!matrix.count(k))
            matrix[k] = vector<bool>(11, false);
        matrix[k][v] = true;
    }
    int count = 0;
    for (auto &val : matrix)
    // for (int i = 1; i <= n; ++i)
    {
        auto i = val.first;

        bool is1 = true, is2 = true, is3 = true;
        // 2,3,4,5
        for (int j = 2; j <= 5; ++j)
        {
            if (matrix[i][j] == true)
            {
                is1 = false;
                break;
            }
        }

        // 4,5,6,7
        for (int j = 4; j <= 7; ++j)
        {
            if (matrix[i][j] == true)
            {
                is2 = false;
                break;
            }
        }

        // 6 7 8 9
        for (int j = 6; j <= 9; ++j)
        {
            if (matrix[i][j] == true)
            {
                is3 = false;
                break;
            }
        }

        count += is1 + is2 + is3;
        if ((is1 && is2) || (is2 && is3))
        {
            count--;
        }
    }
    count += (n - matrix.size()) * 2;
    return count;
}
int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    vector<vector<int>> a = {{1, 2},
                             {1, 3},
                             {1, 8},
                             {2, 6},
                             {3, 1},
                             {3, 10}};
    cout << maxNumberOfFamilies(3, a);
    return 0;
}
