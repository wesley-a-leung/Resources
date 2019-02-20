#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the length of the longest increasing subsequence
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct LongestIncreasingSubsequence {
    T A[MAXN], dp[MAXN];
    int solve(int N) {
        int ret = 0;
        for (int i = 0, j; i < N; i++) {
            dp[j = lower_bound(dp, dp + ret, A[i]) - dp] = A[i];
            if (j == ret) ret++;
        }
        return ret;
    }
};
