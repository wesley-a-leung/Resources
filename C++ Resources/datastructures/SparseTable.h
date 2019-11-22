#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sparse Table
// Time Complexity:
//   init: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
template <const int MAXN, const int MAXLGN> struct SparseTableRMQ {
    using Data = int; Data ST[MAXLGN][MAXN];
    // operation must be associative and idempotent
    Data merge(const Data &l, const Data &r); // to be implemented
    template <class It> void init(It st, It en) {
        int N = en - st, lg = 32 - __builtin_clz(N); assert(lg < MAXLGN); copy(st, en, ST[0]);
        for (int i = 0; i < lg - 1; i++) for (int j = 0; j < N; j++) ST[i + 1][j] = merge(ST[i][j], ST[i][min(j + (1 << i), N - 1)]);
    }
    // 0-indexed, inclusive
    Data query(int l, int r) { int i = 31 - __builtin_clz(r - l + 1); return merge(ST[i][l], ST[i][r - (1 << i) + 1]); }
};
