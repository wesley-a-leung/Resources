#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates
//   and point queries in any number of dimensions
// Indices are 0-indexed and ranges are inclusive
// Order of arguments for update differs slightly from FenwickTreeRangePoint1D
// Template Arguments:
//   D: the number of dimensions of the fenwick tree
//   T: the type of each element
// Constructor Arguments:
//   N: the size of the first dimension
//   ...args: the sizes of the second, third, fourth, etc. dimensions
// Functions:
//   update(v, l, r, ...args): queries the range [l, r] in the first dimension,
//     and alternating arguments in args for beginning and ending ranges for
//     the next dimensions with the value v
//   get(i, ...args): queries the index i in the first
//     dimension, and ...args for the next dimensions
// In practice, this version performs as well as the 1D version
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor: O(PI(N_i))
//   update: O(2^D * PI(log(N_i)))
//   getValue: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
//   PI is the product function, N_i is the size in the ith dimension,
//   and D is the number of dimensions
// Tested:
//   Fuzz and Stress Tested
//   https://mcpt.ca/problem/adifferenceproblem
template <const int D, class T> struct FenwickTreeRangePoint {
  int N; vector<FenwickTreeRangePoint<D - 1, T>> BIT;
  template <class ...Args> FenwickTreeRangePoint(int N, Args &&...args)
    : N(N), BIT(N + 1, FenwickTreeRangePoint<D - 1, T>(
          forward<Args>(args)...)) {}
  template <class ...Args> void update(T v, int l, int r, Args &&...args) {
    for (l++; l <= N; l += l & -l) BIT[l].update(v, forward<Args>(args)...);
    for (r += 2; r <= N; r += r & -r)
      BIT[r].update(-v, forward<Args>(args)...);
  }
  template <class ...Args> T get(int i, Args &&...args) {
    T ret = T();
    for (i++; i > 0; i -= i & -i) ret += BIT[i].get(forward<Args>(args)...);
    return ret;
  }
};

template <class T> struct FenwickTreeRangePoint<0, T> {
  T val; FenwickTreeRangePoint() : val(T()) {}
  void update(T v) { val += v; }
  T get() { return val; }
};
