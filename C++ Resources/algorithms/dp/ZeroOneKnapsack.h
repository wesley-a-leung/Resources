#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum value that can be obtained by items in a knapsack (from a selection of N items)
// that can hold a maximum of M weight, not allowing for repeated instances of items
// Time Complexity: O(NM)
// Space Complexity: O(N + M)
template <const int MAXN, const int MAXM, class value_type> struct ZeroOneKnapsack {
    int W[MAXN]; value_type V[MAXN], dp[MAXM];
    value_type solve(int N, int M) {
        fill(dp, dp + M + 1, 0);
        for (int i = 0; i < N; i++) for (int j = M; j >= W[i]; j--) dp[j] = max(dp[j - W[i]] + V[i]);
        return dp[M];
    }
};
