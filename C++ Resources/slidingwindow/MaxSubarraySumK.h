#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of a subarray of size K
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct MaxSubarraySumK {
    T maxSum, A[MAXN]; int st, en;
    T solve(int N, int K) {
        maxSum = 0; st = 0; en = K;
        for (int i = 0; i < K; i++) maxSum += A[i];
        T curSum = maxSum;
        for (int i = K; i < N; i++) if ((curSum += A[i] - A[i - K]) > maxSum) { st = i - K + 1; en = i + 1; maxSum = curSum; }
        return maxSum;
    }
};
