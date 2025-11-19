#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//maximum carrots the rabbit can collect
int maxCarrots(const vector<vector<int>>& carrotsRight,
    const vector<vector<int>>& carrotsDown,
    int n)
{
    vector<vector<int>> DP(n, vector<int>(n, 0));

    DP[0][0] = 0; //base case

    for (int j = 1; j < n; j++) {
        DP[0][j] = DP[0][j - 1] + carrotsRight[0][j - 1];
    }

    for (int i = 1; i < n; i++) {
        DP[i][0] = DP[i - 1][0] + carrotsDown[i - 1][0];
    }

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            int fromAbove = DP[i - 1][j] + carrotsDown[i - 1][j];
            int fromLeft = DP[i][j - 1] + carrotsRight[i][j - 1];
            DP[i][j] = max(fromAbove, fromLeft);
        }
    }

    return DP[n - 1][n - 1];
}

int main() {
    int n = 3;

    //random carrot matrices
    vector<vector<int>> carrotsRight = {
        {2, 1, 0},
        {4, 3, 2},
        {1, 2, 5}
    };

    vector<vector<int>> carrotsDown = {
        {3, 0, 4},
        {2, 1, 3},
        {0, 2, 0}
    };

    cout << "maximum carrots collected = "
        << maxCarrots(carrotsRight, carrotsDown, n)
        << endl;

    return 0;
}
