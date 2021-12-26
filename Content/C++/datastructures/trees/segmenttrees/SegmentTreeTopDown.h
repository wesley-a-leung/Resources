#pragma once
#include <bits/stdc++.h>
using namespace std;

// Top down segment tree supporting point updates and range queries,
//   with a lazy template argument to enable range updates
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   LAZY: boolean to indicate whether or not range updates are enabled
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static ldef(): only required if LAZY is true, returns the lazy
//         default value of type Lazy
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r): only required if LAZY is false, returns the
//         value r of type Lazy applied to l of type Data, must be associative
//       static applyLazy(l, r, k): only required if LAZY is true, returns the
//         value r of type Lazy applied to l of type Data over a segment of
//         length k, must be associative
//       static mergeLazy(l, r): only required if LAZY is true, returns
//         the values l of type Lazy merged with r of type Lazy,
//         must be associative
//     Sample Struct: C1 supports point increments and range max queries,
//         while C2 supports range assignments and range sum queries
//       struct C1 {
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
//       struct C2 {
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
//   update(i, v): updates the index i with the lazy value v
//   update(l, r, v): update the range [l, r] with the lazy value v
//   query(l, r): queries the range [l, r] and returns the aggregate value
//   bsearchPrefix(l, r, f): returns the smallest index i in the range [l, r]
//     such that f(query(l, i)) returns true or r + 1 if none exist
//   bsearchSuffix(l, r, f): returns the largest index i in the range [l, r]
//     such that f(query(i, r)) returns true or l - 1 if none exist
// In practice, has a small constant, not quite as fast as fenwick trees
//   or bottom up non lazy segment trees,
//   and similar performance as bottom up lazy segment trees
// Time Complexity:
//   constructor: O(N)
//   update, query, bsearchPrefix, bsearchSuffix: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds3 (LAZY = false)
//   https://dmoj.ca/problem/dmpg17g2 (LAZY = false)
//   https://dmoj.ca/problem/coci17c1p5 (LAZY = false, bsearchPrefix)
//   https://judge.yosupo.jp/problem/point_set_range_composite (LAZY = false)
//   https://dmoj.ca/problem/lazy (LAZY = true)
//   https://mcpt.ca/problem/seq3 (LAZY = true)
//   https://judge.yosupo.jp/problem/range_affine_range_sum (LAZY = true)
//   https://dmoj.ca/problem/dmopc17c4p6 (LAZY = true)
template <const bool LAZY, class C> struct SegmentTreeTopDown {
#define lazy_def template <const bool _ = LAZY> typename enable_if<_>::type
#define agg_def template <const bool _ = LAZY> typename enable_if<!_>::type
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  int N; vector<Data> TR; vector<Lazy> LZ;
  lazy_def apply(int x, int tl, int tr, const Lazy &v) {
    TR[x] = C::applyLazy(TR[x], v, tr - tl + 1);
    if (x < int(LZ.size())) LZ[x] = C::mergeLazy(LZ[x], v);
  }
  agg_def apply(int x, int, int, const Lazy &v) {
    TR[x] = C::applyLazy(TR[x], v);
  }
  lazy_def propagate(int x, int tl, int tr) {
    if (LZ[x] != C::ldef()) {
      int m = tl + (tr - tl) / 2; apply(x * 2, tl, m, LZ[x]);
      apply(x * 2 + 1, m + 1, tr, LZ[x]); LZ[x] = C::ldef();
    }
  }
  agg_def propagate(int, int, int) {}
  void build(const vector<Data> &A, int x, int tl, int tr) {
    if (tl == tr) { TR[x] = A[tl]; return; }
    int m = tl + (tr - tl) / 2;
    build(A, x * 2, tl, m); build(A, x * 2 + 1, m + 1, tr);
    TR[x] = C::merge(TR[x * 2], TR[x * 2 + 1]);
  }
  void update(int x, int tl, int tr, int l, int r, const Lazy &v) {
    if (l <= tl && tr <= r) { apply(x, tl, tr, v); return; }
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    if (tl <= r && l <= m) update(x * 2, tl, m, l, r, v);
    if (m + 1 <= r && l <= tr) update(x * 2 + 1, m + 1, tr, l, r, v);
    TR[x] = C::merge(TR[x * 2], TR[x * 2 + 1]);
  }
  Data query(int x, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return C::qdef();
    if (l <= tl && tr <= r) return TR[x];
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    return C::merge(query(x * 2, tl, m, l, r),
                    query(x * 2 + 1, m + 1, tr, l, r));
  }
  template <class F>
  int bsearchPrefix(int x, int tl, int tr, int l, int r, Data &agg, F f) {
    if (r < tl || tr < l) return r + 1;
    if (tl != tr) propagate(x, tl, tr);
    if (l <= tl && tr <= r) {
      Data v = C::merge(agg, TR[x]); if (!f(v)) { agg = v; return r + 1; }
    }
    if (tl == tr) return tl;
    int m = tl + (tr - tl) / 2;
    int ret = bsearchPrefix(x * 2, tl, m, l, r, agg, f);
    return ret <= r ? ret : bsearchPrefix(x * 2 + 1, m + 1, tr, l, r, agg, f);
  }
  template <class F>
  int bsearchSuffix(int x, int tl, int tr, int l, int r, Data &agg, F f) {
    if (r < tl || tr < l) return l - 1;
    if (tl != tr) propagate(x, tl, tr);
    if (l <= tl && tr <= r) {
      Data v = C::merge(TR[x], agg); if (!f(v)) { agg = v; return l - 1; }
    }
    if (tl == tr) return tl;
    int m = tl + (tr - tl) / 2;
    int ret = bsearchSuffix(x * 2 + 1, m + 1, tr, l, r, agg, f);
    return l <= ret ? ret : bsearchSuffix(x * 2, tl, m, l, r, agg, f);
  }
  SegmentTreeTopDown(int N)
      : N(N), TR(N == 0 ? 0 : 1 << __lg(N * 4 - 1), C::qdef()) {
    if (LAZY) LZ.resize(N == 0 ? 0 : 1 << __lg(N * 2 - 1), C::ldef());
  }
  SegmentTreeTopDown(const vector<Data> &A) : SegmentTreeTopDown(A.size()) {
    if (N > 0) { build(A, 1, 0, N - 1); }
  }
  lazy_def update(int l, int r, const Lazy &v) {
    update(1, 0, N - 1, l, r, v);
  }
  void update(int i, const Lazy &v) { update(1, 0, N - 1, i, i, v); }
  Data query(int l, int r) { return query(1, 0, N - 1, l, r); }
  template <class F> int bsearchPrefix(int l, int r, F f) {
    Data agg = C::qdef(); return bsearchPrefix(1, 0, N - 1, l, r, agg, f);
  }
  template <class F> int bsearchSuffix(int l, int r, F f) {
    Data agg = C::qdef(); return bsearchSuffix(1, 0, N - 1, l, r, agg, f);
  }
#undef lazy_def
#undef agg_def
};
