#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sparse Table for Range Minimum / Maximum Queries
// Time Complexity:
//   init: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
template <const int MAXN, const int MAXLGN, class T, const bool IS_MAX> struct SparseTableRMQ {
    T A[MAXN]; int ST[MAXLGN][MAXN], LG[MAXN];
    int cmpInd(int l, int r) { return (A[l] <= A[r]) ^ IS_MAX ? l : r; }
    void init(int N) {
        LG[0] = LG[1] = 0;
        for (int i = 2; i <= N; i++) LG[i] = LG[i / 2] + 1;
        int lg = LG[N] + 1; assert(lg < MAXLGN); iota(ST[0], ST[0] + N, 0);
        for (int i = 0; i < lg - 1; i++) for (int j = 0; j + (1 << i) < N; j++) ST[i + 1][j] = cmpInd(ST[i][j], ST[i][j + (1 << i)]);
    }
    // 0-indexed, inclusive
    int query(int l, int r) { int i = LG[r - l + 1]; return cmpInd(ST[i][l], ST[i][r - (1 << i) + 1]); }
};
