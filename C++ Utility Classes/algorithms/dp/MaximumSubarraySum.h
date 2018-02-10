#ifndef ALGORITHMS_DP_MAXIMUMSUBARRAYSUM_H_
#define ALGORITHMS_DP_MAXIMUMSUBARRAYSUM_H_

#include <bits/stdc++.h>
using namespace std;

int maxSum, start, end; // inclusive

/**
 * Computes the subarray with the maximum sum with Kadane's Algorithm.
 *
 * @param A the array
 * @param N the length of the array
 */
void solve(int *A, int N) {
    maxSum = 0;
    start = 0;
    end = 0;
    int curMax = 0;
    int curStart = 0;
    for (int i = 0; i < N; i++) {
        curMax += A[i];
        if (curMax < 0) {
            curMax = 0;
            curStart = i + 1;
        }
        if (maxSum < curMax) {
            maxSum = curMax;
            start = curStart;
            end = i;
        }
    }
}

#endif /* ALGORITHMS_DP_MAXIMUMSUBARRAYSUM_H_ */
