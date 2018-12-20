#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the number of unique zigzag sequences of a specified length N, modulo a number
// A zigzag sequence alternates between increasing and decreasing (it can start with either)
// Time Complexity: O(N^2)
// Memory Complexity: O(N)
template <const int MAXN, const long long MOD> struct UniqueZigZag {
    long long dp[2][MAXN];
    long long solve(int N) {
        dp[0][0] = dp[1][0] = 0; dp[1][1] = 1;
        for (int i = 2; i <= N; i++) for (int j = 1; j <= i; j++) {
            if (i % 2 == 0) { dp[0][j] = (dp[1][j - 1] + dp[0][j - 1]) % MOD; dp[0][j + 1] = dp[0][j]; }
            else { dp[1][j] = (dp[0][i] - dp[0][j - 1] + dp[1][j - 1] + MOD) % MOD; dp[1][j + 1] = dp[1][j]; }
        }
        return dp[N % 2][N] % MOD;
    }
};
