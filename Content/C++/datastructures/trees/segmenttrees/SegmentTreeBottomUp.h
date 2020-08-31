#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bottom up segment tree supporting point updates and range queries
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   C: struct to combine data and lazy values
//     Fields:
//       Data: typedef/using for the data type
//       Lazy: typedef/using for the lazy type
//       qdef: the query default value of type Data
//     Functions:
//       merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//     Sample Struct: supporting point increments and range max queries
//       struct C {
//         using Data = int;
//         using Lazy = int;
//         const Data qdef = numeric_limits<int>::min();
//         Data merge(const Data &l, const Data &r) const { return max(l, r); }
//         Data applyLazy(const Data &l, const Lazy &r) const { return l + r; }
//       };
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   vdef: the default value to fill the array with
// Functions:
//   update(i, v): updates the index i with the lazy value v
//   query(l, r): queries the range [l, r] and returns the aggregate value
// In practice, has a small constant, not quite as fast as fenwick trees,
//   but faster than top down segment trees
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds3
//   https://dmoj.ca/problem/dmpg17g2
//   https://dmoj.ca/problem/coci17c1p5
//   https://dmoj.ca/problem/cco20p5
//   https://www.spoj.com/problems/BRCKTS/
//   https://judge.yosupo.jp/problem/point_set_range_composite
template <class C> struct SegmentTreeBottomUp {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  int N; vector<Data> TR;
  template <class F> SegmentTreeBottomUp(int N, F f)
      : N(N), TR(N * 2, C().qdef) {
    generate(TR.begin() + N, TR.end(), f); for (int i = N - 1; i > 0; i--)
      TR[i] = C().merge(TR[i * 2], TR[i * 2 + 1]);
  }
  template <class It> SegmentTreeBottomUp(It st, It en)
      : SegmentTreeBottomUp(en - st, [&] { return *st++; }) {}
  SegmentTreeBottomUp(int N, const Data &vdef)
      : SegmentTreeBottomUp(N, [&] { return vdef; }) {}
  void update(int i, const Lazy &v) {
    for (i += N, TR[i] = C().applyLazy(TR[i], v); i /= 2;)
      TR[i] = C().merge(TR[i * 2], TR[i * 2 + 1]);
  }
  Data query(int l, int r) {
    Data ql = C().qdef, qr = C().qdef;
    for (l += N, r += N; l <= r; l /= 2, r /= 2) {
      if (l % 2) ql = C().merge(ql, TR[l++]);
      if (!(r % 2)) qr = C().merge(TR[r--], qr);
    }
    return C().merge(ql, qr);
  }
};
