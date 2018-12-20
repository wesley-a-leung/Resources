#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest common subsequence between 2 arrays of integers
// Time Complexity: O(N^2)
// Memory Complexity: O(N^2)
template <const int MAXN, const int MAXM = MAXN> struct LCIS {
    int A[MAXN], B[MAXM], dp[MAXN][MAXM], subsequence[max(MAXN, MAXM)], len;
    int solve(int N, int M) {
        for (int i = 1; i <= N; i++) for (int j = 1; j <= M; j++) {
            if (A[i - 1] == B[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
        len = dp[N][M];
        for (int i = N, j = M, k = len - 1; i > 0 && j > 0;) {
            if (A[i - 1] == B[j - 1]) { subsequence[k--] = A[i - 1]; i--; j--; }
            else if (dp[i - 1][j] > dp[i][j - 1]) i--;
            else j--;
        }
        return len;
    }
};
