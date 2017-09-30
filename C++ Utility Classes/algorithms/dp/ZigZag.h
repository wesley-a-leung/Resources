#ifndef ALGORITHMS_DP_ZIGZAG_H_
#define ALGORITHMS_DP_ZIGZAG_H_

#include <bits/stdc++.h>

using namespace std;

/**
 * Method for determining the length of the longest zigzag subsequence.
 * A zigzag sequence alternates between increasing and decreasing (it can start
 * with either).
 *
 * Takes time proportional to <em>N</em> where N is the length of the original
 * sequence.
 *
 * Takes space proportional to 2<em>N</em>.
 *
 * @param N the length of the sequence
 * @param sequence the array of integers to check
 * @return the length of the longest zigzag subsequence
 */
int solve(int N, int *sequence) {
    int dp[2][N]; // the 2d array that stores the solution of the maximum subsequence ending with index i
                  // the array has a length of 2 because [0][i] stores the maximum solution where the
                  // final difference is positive while [1][i] stores the maximum solution where the final
                  // difference is negative
    dp[0][0] = 1; // the initial lengths must be at least 1 since a single number is considered a zig-zag sequence
    dp[1][0] = 1;
    for (int i = 1; i < N; i++) { // iterating through every number in the sequence to find the maximum subsequence ending with that index

        if (sequence[i] - sequence[i-1] > 0) { // positive difference
            dp[0][i] = max(dp[1][i-1] + 1, dp[0][i-1]);    // the length of the subsequence is the maximum of either the
                                                           // negative solution + 1 (since signs need to alternate), or
                                                           // the current positive solution
            dp[1][i] = dp[1][i-1];    // the current negative solution remains the same
        } else if (sequence[i] - sequence[i-1] < 0) { // negative difference
            dp[1][i] = max(dp[0][i-1] + 1, dp[1][i-1]);    // the length of the subsequence is the maximum of either the
                                                           // positive solution + 1 (since signs need to alternate), or
                                                           // the current negative solution
            dp[0][i] = dp[0][i-1];                         // the current positive solution remains the same
        } else { // in all other cases, the difference is 0, and it should not be included in the zig-zag sequence
            // both positive and negative solutions remain the same
            dp[0][i] = dp[0][i-1];
            dp[1][i] = dp[1][i-1];
        } // if else
    } // for i
    return max(dp[0][N - 1], dp[1][N - 1]);    // the longest solution is the max subsequence that ends
                                               // in the last index of either the positive or negative solution
}

#endif /* ALGORITHMS_DP_ZIGZAG_H_ */
