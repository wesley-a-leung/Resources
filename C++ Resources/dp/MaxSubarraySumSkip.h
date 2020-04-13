#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the subarray with the maximum sum, removing at most one element from the array
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct MaxSubarraySumSkip {
    T A[MAXN], fw[MAXN], bw[MAXN];
    T solve(int N) {
        T curMax = fw[0] = A[0], maxSum = A[0];
        for (int i = 1; i < N; i++) { fw[i] = curMax = max(A[i], curMax + A[i]); maxSum = max(maxSum, curMax); }
        curMax = maxSum = bw[N - 1] = A[N - 1];
        for (int i = N - 2; i >= 0; i--) { bw[i] = curMax = max(A[i], curMax + A[i]); maxSum = max(maxSum, curMax); }
        for (int i = 1; i < N - 1; i++) maxSum = max(maxSum, fw[i - 1] + bw[i + 1]);
        return maxSum;
    }
};
