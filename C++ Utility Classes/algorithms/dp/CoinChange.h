#ifndef ALGORITHMS_DP_COINCHANGE_H_
#define ALGORITHMS_DP_COINCHANGE_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Method to determine the number of ways to make change
 * for {@code N} dollars given the coin values specified in the
 * {@code coins} array. Each coin can be used an infinite amount
 * of times.
 *
 * Takes time proportional to <em>NM</em> where <em>N</em> is the
 * amount of dollars and <em>M</em> is the number of different
 * coin values.
 *
 * Takes space proportional to <em>N</em>.
 *
 * @param N      dollars
 * @param M      number of coin values
 * @param coins  array of coin values
 * @return       the number of ways to make change
 */
int solve(int N, int M, int *coins) {
    int dp[N + 1];
    dp[0] = 1;
    for (int i = 1; i <= N; i++) {
        dp[i] = 0;
    }
    for (int i = 0; i < M; i++) {
        for (int j = 1; j <= N; j++) {
            if (coins[i] <= j) dp[j] += dp[j - coins[i]];
        }
    }
    return dp[N];
}

#endif /* ALGORITHMS_DP_COINCHANGE_H_ */
