#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sparse Table for generic operations
// F must be a binary operator that is associative and idempotent
// Time Complexity:
//   init: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
template <const int MAXN, class T, class F> struct SparseTable {
    T ST[32 - __builtin_clz(MAXN)][MAXN]; F op;
    template <class It> void init(It st, It en, F op = F()) {
        this->op = op; int N = en - st, lg = 32 - __builtin_clz(N); copy(st, en, ST[0]);
        for (int i = 0; i < lg - 1; i++) for (int j = 0; j < N; j++) ST[i + 1][j] = op(ST[i][j], ST[i][min(j + (1 << i), N - 1)]);
    }
    // 0-indexed, inclusive
    T query(int l, int r) { int i = 31 - __builtin_clz(r - l + 1); return op(ST[i][l], ST[i][r - (1 << i) + 1]); }
};
