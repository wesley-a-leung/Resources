#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of a subarray of size K
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN> struct MaxSubarraySumK {
    int maxSum, st, en, A[MAXN];
    int solve(int N, int K) {
        maxSum = 0; st = 0; en = K;
        for (int i = 0; i < K; i++) maxSum += A[i];
        int curSum = maxSum;
        for (int i = K; i < N; i++) if ((curSum += A[i] - A[i - K]) > maxSum) {
            st = i - K + 1; en = i + 1; maxSum = curSum;
        }
        return maxSum;
    }
};
