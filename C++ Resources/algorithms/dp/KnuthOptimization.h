#pragma once
#include <bits/stdc++.h>
using namespace std;

// Knuth's Dynamic Programming Optimization
// Must statisfy dp[i][j] = min(dp[i][k] + dp[k][j] + cost(i, k, j)) for i <= k <= j
// and A[i][j - 1] <= A[i][j] <= A[i + 1][j], where A[i][j] is the optimal value of k
// for dp[i][j]
// Time Complexity: Reduces the runtime from O(N^3) to O(N^2)
// Memory Complexity: O(N^2)
template <const int MAXN, class T> struct KnuthOptimization {
    T dp[MAXN][MAXN], INF; int mid[MAXN][MAXN];
    KnuthOptimization(T INF) : INF(INF) {}
    T cost(int l, int m, int r); // to be implemented
    T solve(int N) {
        for (int l = N - 1; l >= 0; l--) for (int r = l; r <= N; r++) {
            if (r - l < 2) { dp[l][r] = 0; mid[l][r] = l; continue; }
            dp[l][r] = INF;
            for (int m = mid[l][r - 1]; m <= mid[l + 1][r]; m++) {
                int temp = dp[l][m] + dp[m][r] + cost(l, m ,r);
                if (dp[l][r] > temp) { dp[l][r] = temp; mid[l][r] = m; }
            }
        }
        return dp[0][N];
    }
};
