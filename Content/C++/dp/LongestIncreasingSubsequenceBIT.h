#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest increasing subsequence using a BIT
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct LongestIncreasingSubsequenceBIT {
    T A[MAXN], temp[MAXN]; int BIT[MAXN], dp[MAXN], len[MAXN];
    // dp[i] stores index of previous element of LIS ending at index i
    // len[i] stores length of length of LIS ending at index i
    void update(int i, int v) { for (; i < MAXN; i += i & -i) BIT[i] = max(BIT[i], v); }
    int rmq(int i) { int ret = -1; for (; i > 0; i -= i & -i) ret = max(ret, BIT[i]); return ret; }
    int solve(int N) {
        copy(A, A + N, temp); sort(temp, temp + N); int k = unique(temp, temp + N) - temp, ret = 0; fill(BIT, BIT + MAXN, -1);
        for (int i = 0; i < N; i++) A[i] = lower_bound(temp, temp + k, A[i]) - temp + 1;
        for (int i = 0; i < N; i++) { dp[i] = rmq(A[i] - 1); ret = max(ret, len[i] = dp[i] == -1 ? 1 : len[dp[i]] + 1); update(A[i], i); }
        return ret;
    }
};
