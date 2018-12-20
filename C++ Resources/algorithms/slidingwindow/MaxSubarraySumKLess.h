#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum subarray sum of size K or less
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN> struct MaxSubarraySumK {
    int A[MAXN], dq[MAXN];
    int solve(int N, int K) {
        int front = 0, back = 0, ans = 0;
        for (int i = 1; i < N; i++) A[i] += A[i - 1];
        for (int i = 0; i < N; i++) {
            while (back - front > 0 && dq[front] < i - K) front++;
            while (back - front > 0 && A[dq[back - 1]] >= A[i]) back--;
            dq[back++] = i; ans = max(ans, A[i] - A[dq[front]]);
        }
        return ans;
    }
};
