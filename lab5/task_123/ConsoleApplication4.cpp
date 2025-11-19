#include <iostream>
#include <vector>
using namespace std;

// ----------------------------------------------------
// 1. Naive Fibonacci (Exponential recursion)
// ----------------------------------------------------
long long callCountNaive = 0;

long long fib_naive(int n) {
    callCountNaive++;

    if (n == 0) return 0;
    if (n == 1) return 1;

    return fib_naive(n - 1) + fib_naive(n - 2);
}

// ----------------------------------------------------
// 2. Memoized Top-Down Fibonacci
// ----------------------------------------------------
long long callCountMemo = 0;

long long fib_memo(int n, vector<long long>& memo) {
    callCountMemo++;

    if (n == 0) return 0;
    if (n == 1) return 1;

    if (memo[n] != -1)
        return memo[n];

    memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
    return memo[n];
}

// ----------------------------------------------------
// 3. Iterative Bottom-Up Fibonacci (DP) — FASTEST
// ----------------------------------------------------
long long fib_bottom_up(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    vector<long long> dp(n + 1);
    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    return dp[n];
}

// ----------------------------------------------------
// MAIN — Compare All Three Versions
// ----------------------------------------------------
int main() {
    int n = 40;

    // --- Naive ---
    callCountNaive = 0;
    long long naiveResult = fib_naive(n);

    cout << "Naive Fibonacci:" << endl;
    cout << "F(" << n << ") = " << naiveResult << endl;
    cout << "Function calls = " << callCountNaive << endl;
    cout << "----------------------------------" << endl;

    // --- Memoized ---
    callCountMemo = 0;
    vector<long long> memo(n + 1, -1);
    long long memoResult = fib_memo(n, memo);

    cout << "Memoized Fibonacci:" << endl;
    cout << "F(" << n << ") = " << memoResult << endl;
    cout << "Function calls = " << callCountMemo << endl;
    cout << "----------------------------------" << endl;

    // --- Bottom-Up DP ---
    long long bottomUpResult = fib_bottom_up(n);

    cout << "Bottom-Up Fibonacci:" << endl;
    cout << "F(" << n << ") = " << bottomUpResult << endl;
    cout << "(no recursion)" << endl;

    return 0;
}
