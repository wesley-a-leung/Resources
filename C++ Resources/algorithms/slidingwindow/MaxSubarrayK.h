#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum element for each contiguous subarray of size K
// Time Complexity: O(N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct MaxSubarrayK {
    T A[MAXN], ans[MAXN]; int dq[MAXN];
    void solve(int N, int K) {
        int front = 0, back = 0;
        for (int i = 0; i < N; i++) {
            while (back - front > 0 && dq[front] <= i - K) front++;
            while (back - front > 0 && A[dq[back - 1]] <= A[i]) back--;
            dq[back++] = i;
            if (i >= K - 1) ans[i - K + 1] = A[dq[front]];
        }
    }
};
