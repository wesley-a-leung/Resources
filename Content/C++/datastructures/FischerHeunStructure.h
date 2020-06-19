#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fischer Heun Structure supporting range maximum queries on a static array
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// queryInd: returns the index of the maximum element in the subarray [l, r],
//   breaking ties by selecting the first such index
// query: returns the maximum element in the subarray [l, r]
// In practice, the constructor has a moderate constant, and is significantly
//   faster than sparse table's constructor, query has a moderate constant, and
//   is slightly slower than sparse table's query
// Time Complexity:
//   constructor: O(N)
//   query: O(1)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ncco3d2p1
//   https://www.spoj.com/problems/RMQSQ/
//   https://judge.yosupo.jp/problem/staticrmq
template <class T, class Comparator = less<T>> struct FischerHeunStructure {
  int N, M; vector<T> A; vector<uint32_t> mask; vector<vector<int>> ST;
  int lb(uint32_t x) { return x == 0 ? 0 : __lg(x); }
  int cmpInd(int i, int j) { return Comparator()(A[i], A[j]) ? j : i; }
  int small(int r, int sz = 32) {
    return r - lb(sz == 32 ? mask[r] : mask[r] & ((1U << sz) - 1));
  }
  template <class It> FischerHeunStructure(It st, It en)
      : N(en - st), M(N / 32), A(st, en), mask(N),
        ST(lb(M) + 1, vector<int>(M)) {
    uint32_t k = 0; for (int i = 0; i < N; mask[i++] = k |= 1)
      for (k <<= 1; k && cmpInd(i - lb(k & -k), i) == i; k ^= k & -k);
    for (int i = 0; i < M; i++) ST[0][i] = small(32 * (i + 1) - 1);
    for (int i = 0; i < int(ST.size()) - 1; i++) for (int j = 0; j < M; j++)
      ST[i + 1][j] = cmpInd(ST[i][j], ST[i][min(j + (1 << i), M - 1)]);
  }
  int queryInd(int l, int r) {
    if (r - l + 1 <= 32) return small(r, r - l + 1);
    int ql = small(l + 32 - 1), qr = small(r); l = l / 32 + 1; r = r / 32 - 1;
    if (l <= r) {
      int i = lb(r - l + 1);
      ql = cmpInd(ql, cmpInd(ST[i][l], ST[i][r - (1 << i) + 1]));
    }
    return cmpInd(ql, qr);
  }
  T query(int l, int r) { return A[queryInd(l, r)]; }
};
