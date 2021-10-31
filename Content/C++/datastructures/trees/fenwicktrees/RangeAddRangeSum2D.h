#pragma once
#include <bits/stdc++.h>
#include "FenwickTreeRangePoint.h"
using namespace std;

// Supports range additions and range sum queries in 2D where
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of the element of the grid
// Constructor Arguments:
//   N: the size of the first dimension of the array
//   M: the size of the second dimension of the array
// Functions:
//   update(u, d, l, r, v): adds v to all elements in the range [u, d] in the
//     first dimension and [l, r] in the second dimension
//   query(d, r): queries the sum of the range [0, d] in the
//     first dimension and [0, r] in the second dimension
//   query(d, l, r): queries the sum of the range [0, d] in the
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
  int N, M; vector<FenwickTreeRangePoint<2, T>> FT;
  RangeAddRangeSum2D(int N, int M)
      : N(N), M(M), FT(4, FenwickTreeRangePoint<2, T>(N, M)) {}
  void update(int u, int d, int l, int r, const T &v) {
    FT[0].update(v, u, d, l, r);
    FT[1].update(-v * T(l - 1), u, d, l, r);
    FT[1].update(v * T(r - l + 1), u, d, r + 1, M - 1);
    FT[2].update(-v * T(u - 1), u, d, l, r);
    FT[2].update(v * T(d - u + 1), d + 1, N - 1, l, r);
    FT[3].update(v * T(u - 1) * T(l - 1), u, d, l, r);
    FT[3].update(-v * T(u - 1) * T(r - l + 1), u, d, r + 1, M - 1);
    FT[3].update(-v * T(l - 1) * T(d - u + 1), d + 1, N - 1, l, r);
    FT[3].updateSuffix(v * T(d - u + 1) * T(r - l + 1), d + 1, r + 1);
  }
  T query(int d, int r) {
    return FT[0].get(d, r) * T(d) * T(r) + FT[1].get(d, r) * T(d)
        + FT[2].get(d, r) * T(r) + FT[3].get(d, r);
  }
  T query(int d, int l, int r) { return query(d, r) - query(d, l - 1); }
  T query(int u, int d, int l, int r) {
    return query(d, l, r) - query(u - 1, l, r);
  }
};
