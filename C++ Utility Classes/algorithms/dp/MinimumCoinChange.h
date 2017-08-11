/*
 * MinimumCoinChange.h
 *
 *  Created on: Aug 10, 2017
 *      Author: Wesley Leung
 */

#ifndef ALGORITHMS_DP_MINIMUMCOINCHANGE_H_
#define ALGORITHMS_DP_MINIMUMCOINCHANGE_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * Method to determine the minimum number of coins to make change
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
 * @return       the minimum number of coins to make change
 */
int solve(int N, int M, int *coins) {
    int dp[N + 1];
    dp[0] = 0;
    for (int i = 1; i <= N; i++) {
        dp[i] = INT_MAX;
    }
    for (int i = 1; i <= N; i++) {
        for (int j = 0; j < M; j++) {
            if (coins[j] <= i) {
                int prev = dp[i - coins[j]];
                if (prev != INT_MAX && prev + 1 < dp[i]) dp[i] = prev + 1;
            }
        }
    }
    return dp[N];
}

#endif /* ALGORITHMS_DP_MINIMUMCOINCHANGE_H_ */
