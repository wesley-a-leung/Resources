#ifndef ALGORITHMS_DP_UNBOUNDEDKNAPSACK_H_
#define ALGORITHMS_DP_UNBOUNDEDKNAPSACK_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * Method for computing the maximum value that you can obtain from
 * putting {@code N} elements in a knapsack that can hold a maximum
 * of {@code M} weight, allowing for repeated instances of items.
 *
 * Takes time proportional to <em>NM</em> where <em>N</em> is the number of
 * items and <em>M</em> is the size of the knapsack.
 *
 * Takes space proportional to <em>M</em>.
 *
 * @param N Number of items in the knapsack
 * @param M Size of the knapsack
 * @param weight Array of the weights of the items
 * @param value Array of the values of the items
 * @return the maximum value of the knapsack
 */
int solve(int N, int M, int *weight, int *value) {
    int dp[M + 1];
    for (int j = 0; j <= M; j++) {
        for (int i = 0; i < N; i++) {
            if (weight[i] <= j) dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
        }
    }
    return dp[M];
}

#endif /* ALGORITHMS_DP_UNBOUNDEDKNAPSACK_H_ */
