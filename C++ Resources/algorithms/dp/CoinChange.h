#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the number of ways to make chance for exactly T dollars given N coin values
// Time Complexity: O(TN)
// Memory Complexity: O(T + N)
template <const int MAXT, const int MAXN> struct CoinChange {
    int dp[MAXT], C[MAXN];
    int solve(int T, int N) {
        dp[0] = 1; fill(dp + 1, dp + T + 1, 0);
        for (int i = 0; i < N; i++) for (int j = 1; j <= T; j++) if (C[i] <= j) dp[j] += dp[j - C[i]];
        return dp[T];
    }
};
