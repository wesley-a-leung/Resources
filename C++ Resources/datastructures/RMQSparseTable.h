#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sparse Table for Range Minimum Queries
// Time Complexity:
//   init: O(N log N)
//   query: O(1)
template <const int MAXN, const int MAXLGN, class T> struct RMQSparseTable {
    T A[MAXN]; int ST[MAXLGN][MAXN];
    int minInd(int l, int r) { return A[l] <= A[r] ? l : r; }
    void init(int N) {
        int lg = 32 - __builtin_clz(N); assert(lg < MAXLGN); iota(ST[0], ST[0] + N, 0);
        for (int i = 0; i < lg - 1; i++) for (int j = 0; j + (1 << i) < N; j++)
            ST[i + 1][j] = minInd(ST[i][j], ST[i][j + (1 << i)]);
    }
    int query(int l, int r) { // zero-indexed, inclusive
        int i = 31 - __builtin_clz(r - l + 1);
        return minInd(ST[i][l], ST[i][r - (1 << i) + 1]);
    }
}; 
