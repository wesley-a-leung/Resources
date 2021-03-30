#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and prefix range queries over a cumulative functor, such as max and min,
//   in any number of dimensions
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   D: the number of dimensions of the fenwick tree
//   T: the type of each element
//   Op: a struct with the cumulative operation
//     Required Functions:
//       operator (l, r): combines the values l and r
// Constructor Arguments:
//   qdef: the identity element of the operation
//   N: the size of the first dimension
//   ...args: the sizes of the second, third, fourth, etc. dimensions
// Functions:
//   update(i, ...args): updates the index i in the first dimension,
//     and ...args for the next dimensions, with the last argument
//     being the value to update the index
//   query(r, ...args): queries the cumulative value of the range [0, r] in
//     the first dimension, with the remaining arguments for the
//     next dimensions
// In practice, this version performs as well as the 1D version
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor: O(PI(N_i))
//   update, query: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// where PI is the product function, N_i is the size in the ith dimension
// Tested:
//   https://dmoj.ca/problem/ccc19s5
template <const int D, class T, class Op> struct FenwickTreeCumulative {
  int N; T qdef; vector<FenwickTreeCumulative<D - 1, T, Op>> BIT;
  template <class ...Args> FenwickTreeCumulative(T qdef, int N, Args &&...args)
      : N(N), qdef(qdef), BIT(N + 1, FenwickTreeCumulative<D - 1, T, Op>(
            qdef, forward<Args>(args)...)) {}
  template <class ...Args> void update(int i, Args &&...args) {
    for (i++; i <= N; i += i & -i) BIT[i].update(forward<Args>(args)...);
  }
  template <class ...Args> T query(int r, Args &&...args) {
    T ret = qdef; for (r++; r > 0; r -= r & -r)
      ret = Op()(ret, BIT[r].query(forward<Args>(args)...));
    return ret;
  }
};

template <class T, class Op> struct FenwickTreeCumulative<0, T, Op> {
  T val; FenwickTreeCumulative(T qdef) : val(qdef) {}
  void update(T v) { val = Op()(val, v); }
  T query() { return val; }
};
