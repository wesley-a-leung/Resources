#pragma once
#include <bits/stdc++.h>
#include "FenwickTree1D.h"
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates
//   and range queries in 1 dimension
// Indices are 0-indexed and ranges are inclusive
// numeric_limits<T>::max() * N must not overflow
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Functions:
//   update(l, r, v): add v to the range [l..r]
//   query(r): queries the range [0, r]
//   query(l, r): queries the range [l, r]
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=973
template <class T> struct FenwickTreeRange1D {
  vector<FenwickTree1D<T>> FT;
  FenwickTreeRange1D(int N) : FT(2, FenwickTree1D<T>(N)) {}
  template <class F> FenwickTreeRange1D(int N, F f) {
    FT.reserve(2); FT.emplace_back(N, f); FT.emplace_back(N);
  }
  template <class It> FenwickTreeRange1D(It st, It en) {
    FT.reserve(2); FT.emplace_back(st, en); FT.emplace_back(en - st);
  }
  T query(int r) { return FT[1].query(r) * T(r) + FT[0].query(r); }
  T query(int l, int r) { return query(r) - query(l - 1); }
  void update(int l, int r, T v) {
    FT[1].update(l, v); FT[1].update(r + 1, -v);
    FT[0].update(l, v * T(1 - l)); FT[0].update(r + 1, v * T(r));
  }
};
