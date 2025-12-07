#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip> 

void printDPTable(const std::vector<std::vector<int>>& dp, int n) {
    std::cout << "\n----- Current DP Table -----\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cout << std::setw(4) << dp[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------\n";
}

int solveRabbitProblem(const std::vector<std::vector<int>>& carrots_down, const std::vector<std::vector<int>>& carrots_right, int n)
{
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

    std::cout << "Step 1: Initializing DP table with all zeros." << std::endl;
    printDPTable(dp, n);

    std::cout << "Step 2: Calculating base cases (first row)." << std::endl;
    for (int j = 1; j < n; ++j) {
        dp[0][j] = dp[0][j - 1] + carrots_right[0][j - 1];
        std::cout << "  - To reach (0, " << j << "), rabbit must come from the left. Carrots = " << dp[0][j - 1] << " + " << carrots_right[0][j - 1] << " = " << dp[0][j] << std::endl;
    }
    printDPTable(dp, n);

    std::cout << "Step 3: Calculating base cases (first column)." << std::endl;
    for (int i = 1; i < n; ++i) {
        dp[i][0] = dp[i - 1][0] + carrots_down[i - 1][0];
        std::cout << "  - To reach (" << i << ", 0), rabbit must come from above. Carrots = " << dp[i - 1][0] << " + " << carrots_down[i - 1][0] << " = " << dp[i][0] << std::endl;
    }
    printDPTable(dp, n);

    std::cout << "Step 4: Filling the rest of the DP table using the recurrence relation." << std::endl;
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < n; ++j) {
            int from_above = dp[i - 1][j] + carrots_down[i - 1][j];
            int from_left = dp[i][j - 1] + carrots_right[i][j - 1];
            
            dp[i][j] = std::max(from_above, from_left);

            std::cout << "  - For cell (" << i << ", " << j << "):" << std::endl;
            std::cout << "    - Path from above: " << dp[i - 1][j] << " + " << carrots_down[i - 1][j] << " = " << from_above << std::endl;
            std::cout << "    - Path from left:  " << dp[i][j - 1] << " + " << carrots_right[i][j - 1] << " = " << from_left << std::endl;
            std::cout << "    - Maximum carrots to reach (" << i << ", " << j << ") is " << dp[i][j] << std::endl;
        }
    }
    
    std::cout << "\nStep 5: Final DP table is complete." << std::endl;
    printDPTable(dp, n);

    return dp[n - 1][n - 1];
}

int main()
{
    const int n = 4;

    const std::vector<std::vector<int>> carrots_down = {
        {1, 4, 2, 8},
        {5, 3, 6, 7},
        {2, 8, 4, 1},
        {9, 3, 5, 2}
    };

    const std::vector<std::vector<int>> carrots_right = {
        {3, 2, 5, 9},
        {1, 7, 3, 4},
        {6, 2, 8, 5},
        {3, 4, 7, 1}
    };

    std::cout << "=======================================" << std::endl;
    std::cout << "  Solving the Rabbit Problem with DP   " << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "Grid size: " << n << "x" << n << "\n" << std::endl;

    int maxCarrots = solveRabbitProblem(carrots_down, carrots_right, n);

    std::cout << "\n=======================================" << std::endl;
    std::cout << "             SOLUTION FOUND            " << std::endl;
    std::cout << "=======================================" << std::endl;
    std::cout << "The maximum number of carrots the rabbit can collect is: " << maxCarrots << std::endl;
    std::cout << "This is the value in the bottom-right corner of the final DP table." << std::endl;
    std::cout << "=======================================\n" << std::endl;

    return 0;
}