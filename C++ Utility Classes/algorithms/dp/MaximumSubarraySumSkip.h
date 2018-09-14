#ifndef ALGORITHMS_DP_MAXIMUMSUBARRAYSUMSKIP_H_
#define ALGORITHMS_DP_MAXIMUMSUBARRAYSUMSKIP_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Computes the maximum subarray sum removing at most one element from the array with Kadane's Algorithm.
 *
 * @param a the array
 * @param N the length of the array
 * @return the maximum subarray sum removing at most one element from the array
 */
int solve(int *A, int N) {
    int fw[N], bw[N];
    int curMax = A[0];
    int maxSum = A[0];
    fw[0] = A[0];
    for (int i = 1; i < N; i++) {
        curMax = max(A[i], curMax + A[i]);
        maxSum = max(maxSum, curMax);
        fw[i] = curMax;
    }
    curMax = maxSum = bw[N - 1] = A[N - 1];
    for (int i = N - 2; i >= 0; i--) {
        curMax = max(A[i], curMax + A[i]);
        maxSum = max(maxSum, curMax);
        bw[i] = curMax;
    }
    for (int i = 1; i < N - 1; i++) maxSum = max(maxSum, fw[i - 1] + bw[i + 1]); // excluding ith index
    return maxSum;
}

#endif /* ALGORITHMS_DP_MAXIMUMSUBARRAYSUMSKIP_H_ */
