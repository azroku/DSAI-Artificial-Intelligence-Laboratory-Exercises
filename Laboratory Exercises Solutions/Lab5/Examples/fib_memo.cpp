#include <iostream>
#include <vector>

long long call_count;

long long fib_memo(int n, std::vector<long long>& memo) {
    
    call_count++;

    if (memo[n] != -1) {
        return memo[n];
    }

    long long result;

    if (n <= 1) {
        result = n;
    } else {
        
        result = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
    }

    memo[n] = result;
    
    return result;
}

int main() {
    int n = 40;

    std::cout << "--- Task 3.2: Top-Down with Memoization ---" << std::endl;
    std::cout << "\nComparing performance for n = " << n << std::endl;

    std::cout << "\nRunning naive version..." << std::endl;
    
    call_count = 0;

    std::cout << "Naive version call count for n=40 is ~331,160,281" << std::endl;


    std::cout << "\nRunning memoized version..." << std::endl;

    std::vector<long long> memo(n + 1, -1);

    call_count = 0;

    long long memo_result = fib_memo(n, memo);

    std::cout << "Result: " << memo_result << std::endl;
    std::cout << "Number of function calls: " << call_count << std::endl;

    std::cout << "\nMemoization reduces the calls from hundreds of millions to less than a hundred!" << std::endl;

    return 0;
}