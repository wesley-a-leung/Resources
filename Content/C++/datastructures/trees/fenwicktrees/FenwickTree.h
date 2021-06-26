#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and range queries over a cumulative functor, such as sum, max, and min,
//   in any number of dimensions
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   D: the number of dimensions of the fenwick tree
//   T: the type of each element
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r
//   Inv: a struct with the inverse cumulative operation (minus<T> by default),
//       if Op has no inverse, some dummy struct may need to be provided
//     Required Functions:
//       operator (l, r): performs the inverse operation on values l and r
// Constructor Arguments:
//   N: the size of the first dimension
//   ...args: the sizes of the second, third, fourth, etc. dimensions, with
//     the last value being the identity element (T() by default)
// Functions:
//   update(i, ...args): updates the index i in the first dimension,
//     and ...args for the next dimensions, with the last argument
//     being the value to update the index
//   queryPrefix(r, ...args): queries the cumulative value of the range [0, r]
//     in the first dimension, with the remaining arguments for the
//     next dimensions
//   query(l, r, ...args): queries the cumulative value of the range [l, r]
//     in the first dimension, and alternating arguments in args for beginning
//     and ending ranges for the next dimensions
// In practice, has a small constant, faster than segment trees and comparable
//   to the 1D version
// Time Complexity:
//   constructor: O(PI(N_i))
//   update, queryPrefix: O(PI(log(N_i)))
//   query: O(2^D * PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
//   PI is the product function, N_i is the size in the ith dimension
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/ioi01p1
//   https://dmoj.ca/problem/gfssoc1s4
//   https://dmoj.ca/problem/ccc19s5
template <const int D, class T, class Op = plus<T>, class Inv = minus<T>>
struct FenwickTree {
  int N; vector<FenwickTree<D - 1, T, Op, Inv>> BIT; T val;
  template <class ...Args> FenwickTree(int N, Args &&...args)
      : N(N),
        BIT(N + 1, FenwickTree<D - 1, T, Op, Inv>(forward<Args>(args)...)),
        val(BIT[0].val) {}
  template <class ...Args> void update(int i, Args &&...args) {
    for (i++; i <= N; i += i & -i) BIT[i].update(forward<Args>(args)...);
  }
  template <class ...Args> T queryPrefix(int r, Args &&...args) {
    T ret = val; for (r++; r > 0; r -= r & -r)
      ret = Op()(ret, BIT[r].queryPrefix(forward<Args>(args)...));
    return ret;
  }
  template <class ...Args> T query(int l, int r, Args &&...args) {
    T ret = val; for (; l > 0; l -= l & -l)
      ret = Inv()(ret, BIT[l].query(forward<Args>(args)...));
    for (r++; r > 0; r -= r & -r)
      ret = Op()(ret, BIT[r].query(forward<Args>(args)...));
    return ret;
  }
};

template <class T, class Op, class Inv> struct FenwickTree<0, T, Op, Inv> {
  T val; FenwickTree(T qdef = T()) : val(qdef) {}
  void update(T v) { val = Op()(val, v); }
  T queryPrefix() { return val; }
  T query() { return val; }
};
