#pragma once
#include <bits/stdc++.h>
#include "FenwickTree1D.h"
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates
//   and range sum queries in 1 dimension
// Indices are 0-indexed and ranges are inclusive
// numeric_limits<T>::max() * N must not overflow
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   N: the size of the array
//   A: a vector of type T, memory is saved if this is moved and has
//     a capacity of N + 1
// Functions:
//   update(l, r, v): add v to the range [l..r]
//   query(r): queries the sum of the range [0, r]
//   query(l, r): queries the sum of the range [l, r]
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
  FenwickTreeRange1D(vector<T> A) {
    FT.reserve(2); FT.emplace_back(move(A)); FT.emplace_back(FT[0].N);
  }
  T query(int r) { return FT[1].query(r) * T(r) + FT[0].query(r); }
  T query(int l, int r) { return query(r) - query(l - 1); }
  void update(int l, int r, T v) {
    FT[1].update(l, v); FT[1].update(r + 1, -v);
    FT[0].update(l, v * T(1 - l)); FT[0].update(r + 1, v * T(r));
  }
};
