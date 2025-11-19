#include <iostream>
#include <vector>

long long fib_bottom_up(int n) {
    
    if (n <= 1) {
        return n;
    }

    std::vector<long long> dp(n + 1);

    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; ++i) {
        
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    return dp[n];
}

int main() {
    int n = 40;

    std::cout << "--- Task 3.3: Bottom-Up (Tabulation) ---" << std::endl;
    std::cout << "\nCalculating Fibonacci for n = " << n << "..." << std::endl;

    // Call the bottom-up function.
    long long result = fib_bottom_up(n);

    // Report the result.
    std::cout << "Result: " << result << std::endl;
    
    std::cout << "\nThis version is extremely fast and has no recursive overhead." << std::endl;

    return 0;
}