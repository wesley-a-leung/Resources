#ifndef ALGORITHMS_DP_HAMILTONIANPATH_H_
#define ALGORITHMS_DP_HAMILTONIANPATH_H_

#include <bits/stdc++.h>
using namespace std;

int *order;

/**
 * Bitmask dynamic programming solution that finds the shortest Hamiltonian path.
 *
 * Take time proportional to <em>2</em><sup>N</sup><em>N</em><sup>2</sup> where
 * <em>N</em> is the number of vertices.
 */
double solve(int n, double **dist) {
    double minDist = numeric_limits<double>::infinity();
    double dp[1 << n][n];
    order = new int[n];
    for (int i = 0; i < 1 << n; i++)
        for (int j = 0; j < n; j++)
            dp[i][j] = numeric_limits<double>::infinity();

    for (int i = 0; i < n; i++)
        dp[1 << i][i] = 0;

    for (int i = 1; i < 1 << n; i++)
        for (int j = 0; j < n; j++)
            if ((i & 1 << j) != 0)
                for (int k = 0; k < n; k++)
                    if ((i & 1 << k) == 0)
                        dp[i ^ 1 << k][k] = min(dp[i ^ 1 << k][k], dp[i][j] + dist[j][k]);

    for (int i = 0; i < n; i++)
        minDist = min(minDist, dp[(1 << n) - 1][i]);

    // order
    int currState = (1 << n) - 1;
    int last = -1;
    for (int i = n - 1; i >= 0; i--) {
        int next = -1;
        for (int j = 0; j < n; j++)
            if ((currState & 1 << j) != 0 && (next == -1 || dp[currState][j] + (last == -1 ? 0 : dist[j][last]) < dp[currState][next] + (last == -1 ? 0 : dist[next][last])))
                next = j;
        order[i] = last = next;
        currState ^= 1 << last;
    }
    return minDist;
}

#endif /* ALGORITHMS_DP_HAMILTONIANPATH_H_ */
