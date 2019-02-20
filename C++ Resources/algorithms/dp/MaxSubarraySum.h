#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the subarray with the maximum sum using Kadane's Algorithm
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct MaxSubarraySum {
    T A[MAXN], maxSum; int st, en; // st and en are the starting and ending (inclusive) indices of the subarray
    T solve(int N) {
        maxSum = st = 0; en = -1; T curMax = 0; int curSt = 0;
        for (int i = 0; i < N; i++) {
            curMax += A[i];
            if (curMax < 0) { curMax = 0; curSt = i + 1; }
            if (maxSum < curMax) { maxSum = curMax; st = curSt; en = i; }
        }
        return maxSum;
    }
};