#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sparse Table for generic operations
// F must be a binary operator that is associative and idempotent
// Time Complexity:
//   init: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
template <class T, class F> struct SparseTable {
    int N, lg; vector<vector<T>> ST; F op;
    void build() { for (int i = 0; i < lg - 1; i++) for (int j = 0; j < N; j++) ST[i + 1][j] = op(ST[i][j], ST[i][min(j + (1 << i), N - 1)]); }
    template <class It> SparseTable(It st, It en, F op) : N(en - st), lg(32 - __builtin_clz(N)), ST(lg, vector<T>(st, en)), op(op) { build(); }
    template <class It> SparseTable(It st, It en) : SparseTable(st, en, F()) {}
    // 0-indexed, inclusive
    T query(int l, int r) { int i = 31 - __builtin_clz(r - l + 1); return op(ST[i][l], ST[i][r - (1 << i) + 1]); }
};

template <class It, class F> auto makeSparseTable(It st, It en, F op) {
    return SparseTable<typename std::iterator_traits<It>::value_type, F>(st, en, op);
}
