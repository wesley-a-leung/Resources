#pragma once
#include <bits/stdc++.h>
#include "FenwickTreeRangePoint1D.h"
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates
//   and range queries in 1 dimension
// Indices are 0-indexed and ranges are inclusive
// numeric_limits<T>::max() * N must not overflow
// Template Arguments:
//   T: the type of the value
// Constructor Arguments:
//   N: the size of the first dimension
// Functions:
//   update(l, r, v): add v to the range [l..r]
//   query(r): queries the range [0, r]
//   query(l, r): queries the range [l, r]
// Small constant, like most fenwick trees, and faster than segment trees
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=973
template <class T> struct FenwickTreeRange1D {
  vector<FenwickTreeRangePoint1D<T>> FT;
  FenwickTreeRange1D(int N) : FT(2, FenwickTreeRangePoint1D<T>(N)) {}
  T query(int r) { return FT[1].get(r) * T(r) + FT[0].get(r); }
  T query(int l, int r) { return query(r) - query(l - 1); }
  void update(int l, int r, T v) {
    FT[1].update(l, r, v); FT[0].update(l, v * T(1 - l));
    FT[0].update(r + 1, v * T(r));
  }
};
