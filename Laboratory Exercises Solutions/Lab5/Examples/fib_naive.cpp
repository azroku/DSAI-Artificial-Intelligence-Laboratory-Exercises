#include <iostream>
#include <vector>

long long call_count;

long long fib_naive(int n) {
    call_count++;

    if (n <= 1) {
        return n;
    }

    return fib_naive(n - 1) + fib_naive(n - 2);
}

int main() {

    std::vector<int> test_values = {10, 20, 40};

    std::cout << "--- Task 3.1: Naive Recursive Fibonacci Implementation ---" << std::endl;

    for (int n : test_values) {
        call_count = 0;

        std::cout << "\nCalculating Fibonacci for n = " << n << "..." << std::endl;

        long long result = fib_naive(n);

        std::cout << "Result: " << result << std::endl;
        std::cout << "Number of function calls: " << call_count << std::endl;
    }
    
    std::cout << "\nNotice how rapidly the number of calls increases!" << std::endl;

    return 0;
}