#ifndef ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMKMORE_H_
#define ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMKMORE_H_

#include <bits/stdc++.h>
using namespace std;

/**
 * Returns the maximum sum of a subarray of size K or more.
 *
 * @param A the array
 * @param N the length of the array
 * @param K the length of the subarray window
 * @return the maximum sum of a subarray of size K or more
 */
int solve(int *A, int N, int K) {
    int maxSum[N];
    maxSum[0] = A[0];
    int curMax = A[0];
    for (int i = 1; i < N; i++) {
        curMax = max(A[i], curMax + A[i]);
        maxSum[i] = curMax;
    }
    int sum = 0;
    for (int i = 0; i < K; i++) sum += A[i];
    int ans = sum;
    for (int i = K; i < N; i++) {
        sum += A[i] - A[i - K];
        ans = max(ans, max(sum, sum + maxSum[i - K]));
    }
    return ans;
}

#endif /* ALGORITHMS_SLIDINGWINDOW_MAXSUBARRAYSUMKMORE_H_ */
