#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates
//   and point queries, over a cumulative functor, such as sum, max, and min,
//   in any number of dimensions
// Indices are 0-indexed and ranges are inclusive
// Order of arguments for update differs slightly from FenwickTreeRangePoint1D
// Template Arguments:
//   D: the number of dimensions of the fenwick tree
//   T: the type of each element
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutatitve
//   Inv: a struct with the inverse cumulative operation (minus<T> by default),
//       if Op has no inverse, some dummy struct may need to be provided
//     Required Functions:
//       operator (l, r): performs the inverse operation on values l and r
// Constructor Arguments:
//   N: the size of the first dimension
//   ...args: the sizes of the second, third, fourth, etc. dimensions, with
//     the last value being the identity element (T() by default)
// Functions:
//   updateSuffix(v, l, ...args): updates the suffix starting from index l in
//     the first dimension, and ...args the next dimensions with the value a
//     being updated to op(a, v)
//   update(v, l, r, ...args): updates the range [l, r] in the first
//     dimension, and alternating arguments in args for beginning and
//     ending ranges for the next dimensions with the value a being updated to
//     op(a, v)
//   get(i, ...args): queries the value of index i in the first
//     dimension, and ...args for the next dimensions
// In practice, has a small constant, faster than segment trees and comparable
//   to the 1D version
// Time Complexity:
//   constructor: O(PI(N_i))
//   update: O(2^D * PI(log(N_i)))
//   updateSuffix, getValue: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
//   PI is the product function, N_i is the size in the ith dimension,
//   and D is the number of dimensions
// Tested:
//   Fuzz and Stress Tested
//   https://mcpt.ca/problem/adifferenceproblem
template <const int D, class T, class Op = plus<T>, class Inv = minus<T>>
struct FenwickTreeRangePoint {
  int N; vector<FenwickTreeRangePoint<D - 1, T, Op, Inv>> BIT; T val;
  template <class ...Args> FenwickTreeRangePoint(int N, Args &&...args)
    : N(N), BIT(N + 1, FenwickTreeRangePoint<D - 1, T, Op, Inv>(
          forward<Args>(args)...)),
      val(BIT[0].val) {}
  template <class ...Args> void updateSuffix(T v, int l, Args &&...args) {
    for (l++; l <= N; l += l & -l)
      BIT[l].updateSuffix(v, forward<Args>(args)...);
  }
  template <class ...Args> void update(T v, int l, int r, Args &&...args) {
    for (l++; l <= N; l += l & -l) BIT[l].update(v, forward<Args>(args)...);
    for (r += 2; r <= N; r += r & -r)
      BIT[r].update(Inv()(val, v), forward<Args>(args)...);
  }
  template <class ...Args> T get(int i, Args &&...args) {
    T ret = val; for (i++; i > 0; i -= i & -i)
      ret = Op()(ret, BIT[i].get(forward<Args>(args)...));
    return ret;
  }
};

template <class T, class Op, class Inv>
struct FenwickTreeRangePoint<0, T, Op, Inv> {
  T val; FenwickTreeRangePoint(T qdef = T()) : val(qdef) {}
  void updateSuffix(T v) { val = Op()(val, v); }
  void update(T v) { val = Op()(val, v); }
  T get() { return val; }
};
