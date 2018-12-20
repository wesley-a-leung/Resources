#pragma once
#include <bits/stdc++.h>
using namespace std;

// counts the number of partitions of a number such that the sum is equal to n
// Time Complexity: O(N^2)
// Memory Complexity: O(N)
template <const int MAXN> struct Partitions1 {
    long long dp[MAXN];
    long long solve(int N, long long mod) {
        dp[0] = 1;
        for (int i = 1; i <= N; i++) for (int j = i; j <= N; j++) dp[j] = (dp[j] + dp[j - 1]) % mod;
        return dp[N] % mod;
    }
};

// counts the number of partitions of size k, for a number such that the sum is equal to n
// Time Complexity: O(NK)
// Memory Complexity: O(NK)
template <const int MAXN, const int MAXK> struct Partitions2 {
    long long dp[MAXN][MAXK];
    long long solve(int N, int K, long long mod) {
        dp[0][1] = 1;
        for (int i = 1; i <= N; i++) for (int j = 1; j <= min(i, K); j++) dp[i][j] = (dp[i - 1][j - 1] + dp[i - j][j]) % mod;
        return dp[N][K] % mod;
    }
};
