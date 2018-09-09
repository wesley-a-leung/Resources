#ifndef ALGORITHMS_DP_EGGDROPPING_H_
#define ALGORITHMS_DP_EGGDROPPING_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Method for computing the number of trials required to determine
 * which floor out of {@code K} floors will an egg break, using {@code N}
 * eggs, assuming eggs cannot be reused if broken.
 *
 * Takes time proportional to <em>NK^2</em> where <em>N</em> is the number of
 * eggs and <em>K</em> is the number of floors.
 *
 * Takes space proportional to <em>NK</em>.
 *
 * @param N number of Eggs
 * @param K number of Floors
 * @return the number of trials required
 */
int solve(int N, int K) {
    int dp[N + 1][K + 1];
    int res;

    // We need one trial for one floor and 0 trials for 0 floors
    for (int i = 1; i <= N; i++) {
        dp[i][0] = 0;
        dp[i][1] = 1;
    }

    // We always need i trials for one egg and i floors.
    for (int i = 1; i <= K; i++)
        dp[1][i] = i;

    for (int i = 2; i <= N; i++) {
        for (int j = 2; j <= K; j++) {
            dp[i][j] = INT_MAX;
            for (int x = 1; x <= j; x++) {
                res = 1 + max(dp[i - 1][x - 1], dp[i][j - x]);
                if (res < dp[i][j])
                    dp[i][j] = res;
            }
        }
    }
    return dp[N][K];
}

#endif /* ALGORITHMS_DP_EGGDROPPING_H_ */
