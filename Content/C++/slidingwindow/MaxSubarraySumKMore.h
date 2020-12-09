#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum subarray sum of size K or more
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct MaxSubarraySumKMore {
    T A[MAXN], maxSum[MAXN];
    T solve(int N, int K) {
        maxSum[0] = A[0]; T curMax = A[0], sum = 0;
        for (int i = 1; i < N; i++) maxSum[i] = curMax = max(A[i], curMax + A[i]);
        for (int i = 0; i < K; i++) sum += A[i];
        T ans = sum;
        for (int i = K; i < N; i++) { sum += A[i] - A[i - K]; ans = max(ans, max(sum, sum + maxSum[i - K])); }
        return ans;
    }
};
