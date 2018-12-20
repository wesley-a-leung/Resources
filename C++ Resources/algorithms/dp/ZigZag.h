#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the length of the longest zigzag subsequence
// A zigzag sequence alternates between increasing and decreasing (it can start with either)
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN> struct ZigZag {
    int A[MAXN], dp[2][MAXN];
    int solve(int N) {
        dp[0][0] = dp[1][0] = 1;
        for (int i = 1; i < N; i++) {
            if (A[i] > A[i - 1]) { dp[0][i] = max(dp[1][i - 1] + 1, dp[0][i - 1]); dp[1][i] = dp[1][i - 1]; }
            else if (A[i] < A[i - 1]) { dp[1][i] = max(dp[0][i - 1] + 1, dp[1][i - 1]); dp[0][i] = dp[0][i - 1]; }
            else { dp[0][i] = dp[0][i - 1]; dp[1][i] = dp[1][i - 1]; }
        }
        return max(dp[0][N - 1], dp[1][N - 1]);
    }
};
