#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of a non consecutive subsequence
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN> struct MaxNonConsecutiveSum {
    int A[MAXN], dp[MAXN];
    int solve(int N) {
        dp[0] = A[0] > 0 ? A[0] : 0;
        if (N > 1) dp[1] = max(dp[0], A[1]);
        for (int i = 2; i < N; i++) dp[i] = max(dp[i - 2] + A[i], dp[i - 1]);
        return dp[N - 1];
    }
};
