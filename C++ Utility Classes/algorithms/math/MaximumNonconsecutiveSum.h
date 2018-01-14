#ifndef ALGORITHMS_MATH_MAXIMUMNONCONSECUTIVESUM_H_
#define ALGORITHMS_MATH_MAXIMUMNONCONSECUTIVESUM_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * Method for determining the maximum sum of a non consecutive subsequence
 * (no 2 elements chosen may be next to each other).
 *
 * Takes time proportional to <em>N</em> where N is the length of the original
 * sequence.
 *
 * Takes space proportional to <em>N</em>.
 *
 * @param sequence the array of integers to check
 * @return the maximum sum of a non consecutive subsequence
 */
int solve(int *sequence, int n) {
    int dp[n];
    dp[0] = sequence[0];
    dp[1] = max(dp[0], sequence[1]);
    for (int i = 2; i < n; i++) {
        dp[i] = max(dp[i-2] + sequence[i], dp[i-1]);
    }
    return dp[n-1];
}

#endif /* ALGORITHMS_MATH_MAXIMUMNONCONSECUTIVESUM_H_ */
