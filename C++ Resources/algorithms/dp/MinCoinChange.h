#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum number of coins to make change for exactly T dollars given N coin values
// each coin may be used multiple times
// Time Complexity: O(TN)
// Memory Complexity: O(T + N)
template <const int MAXT, const int MAXN> struct CoinChange {
    int dp[MAXT], C[MAXN];
    int solve(int T, int N) {
        dp[0] = 0; fill(dp + 1, dp + T + 1, INT_MAX - 1);
        for (int i = 1; i <= T; i++) for (int j = 0; j < N; j++) if (C[j] <= i) dp[i] = min(dp[i], dp[i - C[j]] + 1); 
        return dp[T];
    }
};
