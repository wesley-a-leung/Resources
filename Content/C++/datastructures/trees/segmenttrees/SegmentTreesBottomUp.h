#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bottom up segment tree supporting point updates and range queries
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// In practice, has a small constant, not quite as fast as fenwick trees,
//   but faster than top down segment trees
// A combine struct is provided as a template parameter
//   with typedefs/using for data and lazy, a query default value (qdef),
//   and implementations of merge and applyLazy
// merge, and applyLazy must both be associative
// Below is a sample struct for point increments and range max queries
// struct Combine {
//   using Data = int;
//   using Lazy = int;
//   const Data qdef = numeric_limits<int>::min();
//   Data merge(const Data &l, const Data &r) const { return max(l, r); }
//   Data applyLazy(const Data &l, const Lazy &r) const { return l + r; }
// };
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
template <class Combine> struct SegmentTreeBottomUp {
  using Data = typename Combine::Data; using Lazy = typename Combine::Lazy;
  Combine C; int N; vector<Data> TR;
  template <class F> SegmentTreeBottomUp(int N, F f)
      : N(N), TR(N * 2, C.qdef) {
    generate(TR.begin() + N, TR.end(), f);
    for (int i = N - 1; i > 0; i--) TR[i] = C.merge(TR[i * 2], TR[i * 2 + 1]);
  }
  template <class It> SegmentTreeBottomUp(It st, It en)
      : SegmentTreeBottomUp(en - st, [&] { return *st++; }) {}
  SegmentTreeBottomUp(int N, const Data &vdef)
      : SegmentTreeBottomUp(N, [&] { return vdef; }) {}
  void update(int i, const Lazy &v) {
    for (i += N, TR[i] = C.applyLazy(TR[i], v); i /= 2;)
      TR[i] = C.merge(TR[i * 2], TR[i * 2 + 1]);
  }
  Data query(int l, int r) {
    Data ql = C.qdef, qr = C.qdef;
    for (l += N, r += N; l <= r; l /= 2, r /= 2) {
      if (l % 2) ql = C.merge(ql, TR[l++]);
      if (!(r % 2)) qr = C.merge(TR[r--], qr);
    }
    return C.merge(ql, qr);
  }
};

// Bottom up segment tree supporting range updates and range queries
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// In practice, has a small constant, not quite as fast as fenwick trees,
//   and similar performance as top down segment trees
// A combine struct is provided as a template parameter
//   with typedefs/using for data and lazy,
//   a query default value (qdef), lazy default value (ldef),
//   and implementations of merge, applyLazy, getSegmentVal, and mergeLazy
// merge, applyLazy, and mergeLazy must all be associative
// Below is a sample struct for range assignment and range sum queries
// struct Combine {
//   using Data = int;
//   using Lazy = int;
//   const Data qdef = 0;
//   const Lazy ldef = numeric_limits<int>::min();
//   Data merge(const Data &l, const Data &r) const { return l + r; }
//   Data applyLazy(const Data &l, const Lazy &r) const { return r; }
//   Lazy getSegmentVal(const Lazy &v, int k) const { return v * k; }
//   Lazy mergeLazy(const Lazy &l, const Lazy &r) const { return r; }
// };
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
      : N(N), lgN(__lg(N)), TR(N * 2, C.qdef), LZ(N, C.ldef) {
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
