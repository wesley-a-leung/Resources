#pragma once
#include <bits/stdc++.h>
using namespace std;

// Disjoint Sparse Table supporting range queries on a static array
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// In practice, constructor has a small constant,
//   query has a large constant, but still faster than segment trees
//   and performs similarly to sparse tables
// op must be an associative operation
// op must also be passed in the constructor with its type specified
//   as a template parameter, and must be constructable; thus, it cannot be
//   a function pointer, and must be a lambda/std::function, or an instance
//   of a functor with the () operator defined (slightly faster in practice)
// Time Complexity:
//   constructor: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.spoj.com/problems/GSS1/
//   https://www.spoj.com/problems/GSS5/
//   https://judge.yosupo.jp/problem/staticrmq
template <class T, class F> struct DisjointSparseTable {
  int N; vector<vector<T>> ST; F op;
  template <class It> DisjointSparseTable(It st, It en, F op)
      : N(en - st), ST(__lg(N * 2 - 1) + 1, vector<T>(st, en)), op(op) {
    for (int i = 1; i < int(ST.size()); i++) {
      for (int j = 0, len = 1 << i; j < N; j += len) {
        int k = min(j + len / 2, N) - 1; T agg = ST[i][k] = ST[0][k];
        for (k--; k >= j; k--) agg = ST[i][k] = op(ST[0][k], agg);
        k = j + len / 2; if (k < N) agg = ST[i][k] = ST[0][k];
        for (k++; k < min(j + len, N); k++) agg = ST[i][k] = op(agg, ST[0][k]);
      }
    }
  }
  T query(int l, int r) {
    if (l == r) return ST[0][l];
    int i = __lg(l ^ r) + 1; return op(ST[i][l], ST[i][r]);
  }
};
