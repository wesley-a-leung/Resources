#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum value that can be obtained by items in a knapsack (from a selection of N items)
// that can hold a maximum of M weight, allowing for repeated instances of items
// Time Complexity: O(NM)
// Space Complexity: O(N + M)
template <const int MAXN, const int MAXM, class TV> struct UnboundedKnapsack {
    int W[MAXN]; TV V[MAXN], dp[MAXM];
    TV solve(int N, int M) {
        for (int j = 0; j <= M; j++) for (int i = 0; i < N; i++) if (W[i] <= j) dp[j] = max(dp[j], dp[j - W[i]] + V[i]);
        return dp[M];
    }
};
