#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the length of the longest increasing subsequence
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN> struct LongestIncreasingSubsequence {
    int A[MAXN], dp[MAXN];
    int solve(int N) {
        int ret = 0;
        for (int i = 0; i < N; i++) {
            int j = lower_bound(dp, dp + ret, A[i]) - dp;
            dp[j] = A[i]; if (j == ret) ret++;
        }
        return ret;
    }
};
