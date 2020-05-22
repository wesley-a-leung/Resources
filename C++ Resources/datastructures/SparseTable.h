#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sparse Table for generic operations
// F must be a binary operator that is associative and idempotent
// Indices are 0-indexed and ranges are inclusive
// In practice, constructor has a small constant, query has a moderate constant, but still faster than segment trees
// Time Complexity:
//   constructor: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
// Tested:
//   https://dmoj.ca/problem/ncco3d2p1
//   https://www.spoj.com/problems/RMQSQ/
template <class T, class F> struct SparseTable {
    int N; vector<vector<T>> ST; F op;
    template <class It> SparseTable(It st, It en, F op) : N(en - st), ST(__lg(N) + 1, vector<T>(st, en)), op(op) {
        for (int i = 0; i < int(ST.size()) - 1; i++) for (int j = 0; j < N; j++) ST[i + 1][j] = op(ST[i][j], ST[i][min(j + (1 << i), N - 1)]);
    }
    T query(int l, int r) { int i = __lg(r - l + 1); return op(ST[i][l], ST[i][r - (1 << i) + 1]); }
};
