#ifndef ALGORITHMS_DP_LONGESTCOMMONINTEGERSUBSEQUENCE_H_
#define ALGORITHMS_DP_LONGESTCOMMONINTEGERSUBSEQUENCE_H_

#include <bits/stdc++.h>

using namespace std;

int *subsequence;
int length;

/**
 * Computes the longest common subsequence between 2 arrays of integers.
 *
 * @param a the first array
 * @param n the length of the first array
 * @param b the second array
 * @param m the length of the second array
 */
void solve(int *a, int n, int *b, int m) {
    int dp[n + 1][m + 1];
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    length = dp[n][m];
    subsequence = new int[length];
    for (int i = n, j = m, k = length - 1; i > 0 && j > 0;) {
        if (a[i - 1] == b[j - 1]) {
            subsequence[k--] = a[i - 1];
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
}

#endif /* ALGORITHMS_DP_LONGESTCOMMONINTEGERSUBSEQUENCE_H_ */
