#ifndef ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMK_H_
#define ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMK_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Returns the maximum sum of a subarray of size K.
 *
 * @param A the array
 * @param N the length of the array
 * @param K the length of the subarray window
 * @param start the reference to the integer to store the starting index (inclusive) of the maximum sum
 * @param end the reference to the integer to store the end index (exclusive) of the maximum sum
 * @return the maximum sum of a subarray of size K
 */
int solve(int *A, int N, int K, int &start, int &end) {
    int maxSum = 0;
    for (int i = 0; i < K; i++) maxSum += A[i];
    start = 0;
    end = K;
    int curSum = maxSum;
    for (int i = K; i < N; i++) {
        curSum += A[i] - A[i - K];
        if (curSum > maxSum) {
            start = i - K + 1;
            end = i + 1;
            maxSum = curSum;
        }
    }
    return maxSum;
}

#endif /* ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMK_H_ */
