#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bottom up segment tree supporting range updates and range queries
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   Combine: struct to combine data and lazy values
//     Fields:
//       Data: typedef/using for the data type
//       Lazy: typedef/using for the lazy type
//       qdef: the query default value of type Data
//     Functions:
//       merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       getSegmentVal(v, k): returns the lazy value v when applied over a
//         segment of length k
//       mergeLazy(l, r): returns the values l of type Lazy merged with
//         r of type Lazy, must be associative
//     Sample Struct: supporting range assignments and range sum queries
//       struct Combine {
//         using Data = int;
//         using Lazy = int;
//         const Data qdef = 0;
//         const Lazy ldef = numeric_limits<int>::min();
//         Data merge(const Data &l, const Data &r) const { return l + r; }
//         Data applyLazy(const Data &l, const Lazy &r) const { return r; }
//         Lazy getSegmentVal(const Lazy &v, int k) const { return v * k; }
//         Lazy mergeLazy(const Lazy &l, const Lazy &r) const { return r; }
//       };
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   vdef: the default value to fill the array with
// Functions:
//   update(l, r, v): update the range [l, r] with the lazy value v
//   query(l, r): queries the range [l, r] and returns the aggregate value
// In practice, has a small constant, not quite as fast as fenwick trees,
//   and similar performance as top down segment trees
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/dmopc17c4p6
//   https://dmoj.ca/problem/dmopc18c5p5
//   https://dmoj.ca/problem/dmopc18c6p5
//   https://dmoj.ca/problem/lazy
//   https://mcpt.ca/problem/seq3
//   https://judge.yosupo.jp/problem/range_affine_range_sum
template <class Combine> struct SegmentTreeLazyBottomUp {
  using Data = typename Combine::Data; using Lazy = typename Combine::Lazy;
  Combine C; int N, lgN; vector<Data> TR; vector<Lazy> LZ;
  void apply(int i, const Lazy &v, int k) {
    TR[i] = C.applyLazy(TR[i], C.getSegmentVal(v, k));
    if (i < N) LZ[i] = C.mergeLazy(LZ[i], v);
  }
  void eval(int i, int k) {
    TR[i] = C.merge(TR[i * 2], TR[i * 2 + 1]);
    if (LZ[i] != C.ldef) TR[i] = C.applyLazy(TR[i], C.getSegmentVal(LZ[i], k));
  }
  void propagate(int i) {
    int h = lgN + 1, k = 1 << lgN, ii = i >> h;
    for (; h > 0; ii = i >> --h, k /= 2) if (LZ[ii] != C.ldef) {
      apply(ii * 2, LZ[ii], k); apply(ii * 2 + 1, LZ[ii], k); LZ[ii] = C.ldef;
    }
  }
  template <class F> SegmentTreeLazyBottomUp(int N, F f)
      : N(N), lgN(N == 0 ? 0 : __lg(N)), TR(N * 2, C.qdef), LZ(N, C.ldef) {
    generate(TR.begin() + N, TR.end(), f);
    for (int i = N - 1; i > 0; i--) TR[i] = C.merge(TR[i * 2], TR[i * 2 + 1]);
  }
  template <class It> SegmentTreeLazyBottomUp(It st, It en)
      : SegmentTreeLazyBottomUp(en - st, [&] { return *st++; }) {}
  SegmentTreeLazyBottomUp(int N, const Data &vdef)
      : SegmentTreeLazyBottomUp(N, [&] { return vdef; }) {}
  void update(int l, int r, const Lazy &v) {
    propagate(l += N); propagate(r += N); bool bl = 0, br = 0; int k = 1;
    for (; l <= r; l /= 2, r /= 2, k *= 2) {
      if (bl) eval(l - 1, k);
      if (br) eval(r + 1, k);
      if (l % 2) { apply(l++, v, k); bl = 1; }
      if (!(r % 2)) { apply(r--, v, k); br = 1; }
    }
    for (l--, r++; r; l /= 2, r /= 2, k *= 2) {
      if (bl) eval(l, k);
      if (br && (!bl || l != r)) eval(r, k);
    }
  }
  Data query(int l, int r) {
    propagate(l += N); propagate(r += N); Data ql = C.qdef, qr = C.qdef;
    for (; l <= r; l /= 2, r /= 2) {
      if (l % 2) ql = C.merge(ql, TR[l++]);
      if (!(r % 2)) qr = C.merge(TR[r--], qr);
    }
    return C.merge(ql, qr);
  }
};
