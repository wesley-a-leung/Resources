#pragma once
#include <bits/stdc++.h>
#include "../fenwicktrees/FenwickTreeRange1D.h"
using namespace std;

// Supports range additions and range sum queries in 2D
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of the element of the grid
// Constructor Arguments:
//   N: the size of the first dimension of the array
//   M: the size of the second dimension of the array
//   A: a 2D vector of elements of type T
// Functions:
//   update(u, d, l, r, v): adds v to all elements in the range [u, d] in the
//     first dimension and [l, r] in the second dimension
//   query(u, d, l, r): queries the sum of the range [u, d] in the
//     first dimension and [l, r] in the second dimension
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(NM)
//   update, query: O(log N log M)
// Memory Complexity: O(NM)
// Tested:
//   Fuzz and Stress Tested
template <class T> struct RangeAddRangeSum2D {
  int N, M; vector<FenwickTreeRange1D<T>> full, part;
  void build(int x, int tu, int td, const vector<vector<T>> &pre) {
    int i = -1; part[x] = FenwickTreeRange1D<T>(M, [&] {
      ++i; return pre[td][i] - (tu == 0 ? T() : pre[tu - 1][i]);
    });
    if (tu == td) {
      i = -1; full[x] = FenwickTreeRange1D<T>(M, [&] {
        ++i; return pre[td][i] - (tu == 0 ? T() : pre[tu - 1][i]);
      });
      return;
    }
    int m = tu + (td - tu) / 2, rc = x + (m - tu + 1) * 2;
    build(x + 1, tu, m, pre); build(rc, m + 1, td, pre);
  }
  void update(int x, int tu, int td, int u, int d, int l, int r, const T &v) {
    part[x].update(l, r, T(min(td, d) - max(u, tu) + 1) * v);
    if (u <= tu && td <= d) { full[x].update(l, r, v); return; }
    int m = tu + (td - tu) / 2, rc = x + (m - tu + 1) * 2;
    if (tu <= d && u <= m) update(x + 1, tu, m, u, d, l, r, v);
    if (m + 1 <= d && u <= td) update(rc, m + 1, td, u, d, l, r, v);
  }
  T query(int x, int tu, int td, int u, int d, int l, int r) {
    if (d < tu || td < u) return T();
    if (u <= tu && td <= d) return part[x].query(l, r);
    int m = tu + (td - tu) / 2, rc = x + (m - tu + 1) * 2;
    return query(x + 1, tu, m, u, d, l, r) + query(rc, m + 1, td, u, d, l, r)
        + T(min(td, d) - max(u, tu) + 1) * full[x].query(l, r);
  }
  RangeAddRangeSum2D(int N, int M)
      : N(N), M(M), full(max(0, N * 2 - 1), FenwickTreeRange1D<T>(M)),
        part(full) {}
  RangeAddRangeSum2D(vector<vector<T>> A)
      : RangeAddRangeSum2D(A.size(), A.empty() ? 0 : A[0].size()) {
    for (int i = 1; i < N; i++) for (int j = 0; j < M; j++)
      A[i][j] += A[i - 1][j];
    if (N > 0 && M > 0) build(0, 0, N - 1, A);
  }
  void update(int u, int d, int l, int r, const T &v) {
    update(0, 0, N - 1, u, d, l, r, v);
  }
  T query(int u, int d, int l, int r) {
    return query(0, 0, N - 1, u, d, l, r);
  }
};
