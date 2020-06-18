#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and range queries in any number of dimensions
// Indices are 0-indexed and ranges are inclusive
// In practice, this version performs as well as the 1D version
// Small constant, like most fenwick trees, and faster than segment trees
// Time Complexity:
//   constructor: O(PI(N_i))
//   update: O(PI(log(N_i)))
//   rsq: O(2^D * PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// PI is the product function, N_i is the size in the ith dimension,
//   and D is the number of dimensions
// Tested:
//   https://dmoj.ca/problem/ds1
//   https://judge.yosupo.jp/problem/point_add_range_sum
//   https://dmoj.ca/problem/ioi01p1
//   https://dmoj.ca/problem/gfssoc1s4
//   https://dmoj.ca/problem/fallingsnowflakes
template <const int D, class T> struct FenwickTree {
  int N; vector<FenwickTree<D - 1, T>> BIT;
  template <class ...Args> FenwickTree(int N, Args &&...args)
      : N(N), BIT(N + 1, FenwickTree<D - 1, T>(forward<Args>(args)...)) {}
  template <class ...Args> void update(int i, Args &&...args) {
    for (i++; i <= N; i += i & -i) BIT[i].update(forward<Args>(args)...);
  }
  template <class ...Args> T rsq(int l, int r, Args &&...args) {
    T ret = T();
    for (; l > 0; l -= l & -l) ret -= BIT[l].rsq(forward<Args>(args)...);
    for (r++; r > 0; r -= r & -r) ret += BIT[r].rsq(forward<Args>(args)...);
    return ret;
  }
};

template <class T> struct FenwickTree<0, T> {
  T val; FenwickTree() : val(T()) {}
  void update(T v) { val += v; }
  T rsq() { return val; }
};
