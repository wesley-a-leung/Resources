#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sparse Table supporting range queries on a static array
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// In practice, constructor has a small constant,
//   query has a large constant, but still faster than segment trees
//   and performs similarly to disjoint sparse tables
// op must be an associative and idempotent operation
// op must also be passed in the constructor with its type specified
//   as a template parameter, and must be constructable; thus, it cannot be
//   a function pointer, and must be a lambda/std::function, or an instance
//   of a functor with the () operator defined (slightly faster in practice)
// Time Complexity:
//   constructor: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
// Tested:
//   https://dmoj.ca/problem/ncco3d2p1
//   https://www.spoj.com/problems/RMQSQ/
//   https://judge.yosupo.jp/problem/staticrmq
template <class T, class F> struct SparseTable {
  int N; vector<vector<T>> ST; F op;
  template <class It> SparseTable(It st, It en, F op)
      : N(en - st), ST(__lg(N) + 1, vector<T>(st, en)), op(op) {
    for (int i = 0; i < int(ST.size()) - 1; i++) for (int j = 0; j < N; j++)
      ST[i + 1][j] = op(ST[i][j], ST[i][min(j + (1 << i), N - 1)]);
  }
  T query(int l, int r) {
    int i = __lg(r - l + 1); return op(ST[i][l], ST[i][r - (1 << i) + 1]);
  }
};
