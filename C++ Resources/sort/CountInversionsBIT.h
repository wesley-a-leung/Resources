#pragma once
#include <bits/stdc++.h>
using namespace std;

// Counts the number of inversions of an array using a BIT
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN> struct CountInversionsBIT {
    int A[MAXN], temp[MAXN], BIT[MAXN];
    void update(int i, int v) { for (; i < MAXN; i += i & -i) BIT[i] += v; }
    int rsq(int i) { int ret = 0; for (; i > 0; i -= i & -i) ret += BIT[i]; return ret; }
    long long solve(int N) {
        copy(A, A + N, temp); sort(temp, temp + N); int k = unique(temp, temp + N) - temp;
        for (int i = 0; i < N; i++) A[i] = lower_bound(temp, temp + k, A[i]) - temp + 1;
        fill(BIT, BIT + MAXN, 0); long long ret = 0;
        for (int i = N - 1; i >= 0; i--) { ret += rsq(A[i] - 1); update(A[i], 1); }
        return ret;
    }
};
