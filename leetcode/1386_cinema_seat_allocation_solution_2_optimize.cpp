#include <bits/stdc++.h>
#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
using namespace std;
int maxNumberOfFamilies(int n, vector<vector<int>> &reservedSeats)
{

    // Có lời giải thích 1386_cinema_seat_allocation_solution_2_optimize.md
    constexpr int BLOCK_A = 0b000000111100;
    constexpr int BLOCK_B = 0b000011110000;
    constexpr int BLOCK_C = 0b001111000000;
    unordered_map<int, int> seats;

    for (auto &rs : reservedSeats)
    {
        int k = rs[0], s = rs[1];
        seats[k] |= (1 << s);
    }
    int result = (n - seats.size()) * 2;
    for (auto &val : seats)
    {
        bool seat_A = (BLOCK_A & val.second) == 0;
        bool seat_B = (BLOCK_B & val.second) == 0;
        bool seat_C = (BLOCK_C & val.second) == 0;
        cout << bitset<11>(BLOCK_A) << " " << bitset<11>(val.second) << " " << seat_A << endl;
        if (seat_A && seat_C)
        {
            result += 2;
        }
        else if (seat_A || seat_B || seat_C)
        {
            result += 1;
        }
    }
    return result;
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
