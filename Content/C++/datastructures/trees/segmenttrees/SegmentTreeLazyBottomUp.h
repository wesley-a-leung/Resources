#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bottom up segment tree supporting range updates and range queries
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static ldef(): returns the lazy default value of type Lazy
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r, k): returns the value r of type Lazy applied to
//         l of type Data over a segment of length k, must be associative
//       static mergeLazy(l, r): returns the values l of type Lazy merged with
//         r of type Lazy, must be associative
//     Sample Struct: supporting range assignments and range sum queries
//       struct C {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         static Lazy ldef() { return numeric_limits<int>::min(); }
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         static Data applyLazy(const Data &l, const Lazy &r, int k) {
//           return r * k;
//         }
//         static Lazy mergeLazy(const Lazy &l, const Lazy &r) { return r; }
//       };
// Constructor Arguments:
//   N: the size of the array
//   A: a vector of type C::Data
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
//   https://dmoj.ca/problem/lazy
//   https://mcpt.ca/problem/seq3
//   https://judge.yosupo.jp/problem/range_affine_range_sum
//   https://dmoj.ca/problem/dmopc17c4p6
template <class C> struct SegmentTreeLazyBottomUp {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  int N, lgN; vector<Data> TR; vector<Lazy> LZ;
  void apply(int i, const Lazy &v, int k) {
    TR[i] = C::applyLazy(TR[i], v, k);
    if (i < N) LZ[i] = C::mergeLazy(LZ[i], v);
  }
  void eval(int i, int k) {
    TR[i] = C::merge(TR[i * 2], TR[i * 2 + 1]);
    if (LZ[i] != C::ldef()) TR[i] = C::applyLazy(TR[i], LZ[i], k);
  }
  void propagate(int i) {
    int h = lgN + 1, k = 1 << lgN, ii = i >> h;
    for (; h > 0; ii = i >> --h, k /= 2) if (LZ[ii] != C::ldef()) {
      apply(ii * 2, LZ[ii], k); apply(ii * 2 + 1, LZ[ii], k);
      LZ[ii] = C::ldef();
    }
  }
  SegmentTreeLazyBottomUp(int N)
      : N(N), lgN(N == 0 ? 0 : __lg(N)),
        TR(N * 2, C::qdef()), LZ(N, C::ldef()) {}
  SegmentTreeLazyBottomUp(const vector<Data> &A)
      : SegmentTreeLazyBottomUp(A.size()) {
    copy(A.begin(), A.end(), TR.begin() + N);
    for (int i = N - 1; i > 0; i--) TR[i] = C::merge(TR[i * 2], TR[i * 2 + 1]);
  }
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
    propagate(l += N); propagate(r += N); Data ql = C::qdef(), qr = C::qdef();
    for (; l <= r; l /= 2, r /= 2) {
      if (l % 2) ql = C::merge(ql, TR[l++]);
      if (!(r % 2)) qr = C::merge(TR[r--], qr);
    }
    return C::merge(ql, qr);
  }
};
