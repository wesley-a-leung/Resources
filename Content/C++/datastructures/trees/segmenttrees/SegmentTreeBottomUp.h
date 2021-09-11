#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bottom up segment tree supporting point updates and range queries
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//     Sample Struct: supporting point increments and range max queries
//       struct C {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return numeric_limits<int>::min(); }
//         static Data merge(const Data &l, const Data &r) {
//           return max(l, r);
//         }
//         static Data applyLazy(const Data &l, const Lazy &r) {
//           return l + r;
//         }
//       };
// Constructor Arguments:
//   N: the size of the array
//   A: a vector of type C::Data, memory is saved if this is moved and has
//     a capacity of 2N
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
//   https://judge.yosupo.jp/problem/point_set_range_composite
template <class C> struct SegmentTreeBottomUp {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  int N; vector<Data> TR;
  SegmentTreeBottomUp(int N) : N(N), TR(N * 2, C::qdef()) {}
  SegmentTreeBottomUp(vector<Data> A) : N(A.size()), TR(move(A))   {
    TR.resize(N * 2, C::qdef()); copy_n(TR.begin(), N, TR.begin() + N);
    for (int i = N - 1; i > 0; i--) TR[i] = C::merge(TR[i * 2], TR[i * 2 + 1]);
  }
  void update(int i, const Lazy &v) {
    for (i += N, TR[i] = C::applyLazy(TR[i], v); i /= 2;)
      TR[i] = C::merge(TR[i * 2], TR[i * 2 + 1]);
  }
  Data query(int l, int r) {
    Data ql = C::qdef(), qr = C::qdef();
    for (l += N, r += N; l <= r; l /= 2, r /= 2) {
      if (l % 2) ql = C::merge(ql, TR[l++]);
      if (!(r % 2)) qr = C::merge(TR[r--], qr);
    }
    return C::merge(ql, qr);
  }
};
