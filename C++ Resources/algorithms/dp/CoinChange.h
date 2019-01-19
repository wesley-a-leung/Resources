#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the number of ways to make change for exactly T dollars given N coin values,
// each coin may be used multiple times
// Time Complexity: O(TN)
// Memory Complexity: O(T + N)
template <const int MAXT, const int MAXN> struct CoinChange {
    int dp[MAXT], C[MAXN];
    int solve(int T, int N) {
        dp[0] = 1; fill(dp + 1, dp + T + 1, 0);
        for (int i = 0; i < N; i++) for (int j = C[i]; j <= T; j++) dp[j] += dp[j - C[i]];
        return dp[T];
    }
};
