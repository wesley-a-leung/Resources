#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and prefix range queries over a cumulative functor, such as max and min,
//   in any number of dimensions
// Indices are 0-indexed and ranges are inclusive
// In practice, this version performs as well as the 1D version
// Small constant, like most fenwick trees, and faster than segment trees
// Time Complexity:
//   constructor: O(PI(N_i))
//   update, query: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// where PI is the product function, N_i is the size in the ith dimension
// Tested:
//   https://dmoj.ca/problem/ccc19s5
template <const int D, class T, class C> struct FenwickTreeCumulative {
  int N; T qdef; vector<FenwickTreeCumulative<D - 1, T, C>> BIT;
  template <class ...Args> FenwickTreeCumulative(T qdef, int N, Args &&...args)
      : N(N), qdef(qdef), BIT(N + 1, FenwickTreeCumulative<D - 1, T, C>(
            qdef, forward<Args>(args)...)) {}
  template <class ...Args> void update(int i, Args &&...args) {
    for (i++; i <= N; i += i & -i) BIT[i].update(forward<Args>(args)...);
  }
  template <class ...Args> T query(int r, Args &&...args) {
    T ret = qdef; for (r++; r > 0; r -= r & -r)
      ret = C()(ret, BIT[r].query(forward<Args>(args)...));
    return ret;
  }
};

template <class T, class C> struct FenwickTreeCumulative<0, T, C> {
  T val; FenwickTreeCumulative(T qdef) : val(qdef) {}
  void update(T v) { val = C()(val, v); }
  T query() { return val; }
};
