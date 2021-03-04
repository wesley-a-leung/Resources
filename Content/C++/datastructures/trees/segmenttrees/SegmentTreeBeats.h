#pragma once
#include <bits/stdc++.h>
using namespace std;

// Top down segment tree supporting advanced range updates and queries
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//       Arg: the argument type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static ldef(): returns the lazy default value of type Lazy
//       staticMakeLazy(d, a): returns a lazy value created from the Data d
//         and Arg a
//       static breakCond(d, a): returns the condition for when to stop the
//         recusion based on Data d and Arg a
//       static tagCond(d, a): returns the condition for when to tag
//         based on Data d and Arg a
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       static getSegmentVal(v, k): returns the lazy value v when applied over
//         a segment of length k
//       static mergeLazy(l, r): returns the values l of type Lazy merged with
//         r of type Lazy, must be associative
//     Sample Struct: supporting range modulo and range sum queries
//       struct C {
//         struct Data { int sm, mx, mn; };
//         using Lazy = pair<int, int>;
//         using Arg = int;
//         static Data qdef() {
//           Data ret; ret.sm = 0;
//           ret.mx = numeric_limits<int>::min();
//           ret.mn = numeric_limits<int>::max();
//           return ret;
//         }
//         static Lazy ldef() { return make_pair(-1, -1); }
//         static Lazy makeLazy(const Data &d, const Arg &a) {
//           return make_pair(a, a);
//         }
//         static bool breakCond(const Data &d, const Arg &a) {
//           return d.mx < a;
//         }
//         static bool tagCond(const Data &d, const Arg &a) {
//           return d.mn == d.mx;
//         }
//         static Data merge(const Data &l, const Data &r) {
//           Data ret; ret.sm = l.sm + r.sm;
//           ret.mx = max(l.mx, r.mx); ret.mn = min(l.mn, r.mn); return ret;
//         }
//         static Data applyLazy(const Data &l, const Lazy &r) {
//           Data ret; ret.sm = r.second; ret.mx = ret.mn = r.first;
//           return ret;
//         }
//         static Lazy getSegmentVal(const Lazy &v, int k) {
//           return make_pair(v.first, v.second * k);
//         }
//         static Lazy mergeLazy(const Lazy &l, const Lazy &r) { return r; }
//       };
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   vdef: the default value to fill the array with
// Functions:
//   update(l, r, v): update the range [l, r] with the Arg v
//   query(l, r): queries the range [l, r] and returns the aggregate value
//   bsearchPrefix(l, r, f): returns the smallest index i in the range [l, r]
//     such that f(query(l, i)) returns true or r + 1 if none exist
//   bsearchSuffix(l, r, f): returns the largest index i in the range [l, r]
//     such that f(query(i, r)) returns true of l - 1 if none exist
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   update: dependent on C
//   query, bsearchPrefix, bsearchSuffix: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/438/problem/D
//   https://judge.yosupo.jp/problem/range_chmin_chmax_add_range_sum
template <class C> struct SegmentTreeBeats {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  using Arg = typename C::Arg;
  struct Node {
    Data val; Lazy lz; Node() : val(C::qdef()), lz(C::ldef()) {}
  };
  int N; vector<Node> TR;
  void apply(int x, int tl, int tr, const Lazy &v) {
    TR[x].val = C::applyLazy(TR[x].val, C::getSegmentVal(v, tr - tl + 1));
    TR[x].lz = C::mergeLazy(TR[x].lz, v);
  }
  void propagate(int x, int tl, int tr) {
    if (TR[x].lz != C::ldef()) {
      int m = tl + (tr - tl) / 2; apply(x * 2, tl, m, TR[x].lz);
      apply(x * 2 + 1, m + 1, tr, TR[x].lz); TR[x].lz = C::ldef();
    }
  }
  template <class F> void build(int x, int tl, int tr, F &f) {
    if (tl == tr) { TR[x].val = f(); return; }
    int m = tl + (tr - tl) / 2;
    build(x * 2, tl, m, f); build(x * 2 + 1, m + 1, tr, f);
    TR[x].val = C::merge(TR[x * 2].val, TR[x * 2 + 1].val);
  }
  void update(int x, int tl, int tr, int l, int r, const Arg &v) {
    if (r < tl || tr < l || C::breakCond(TR[x].val, v)) return;
    if (l <= tl && tr <= r && C::tagCond(TR[x].val, v)) {
      Lazy lz = C::makeLazy(TR[x].val, v);
      TR[x].val = C::applyLazy(TR[x].val, C::getSegmentVal(lz, tr - tl + 1));
      TR[x].lz = C::mergeLazy(TR[x].lz, lz); return;
    }
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    update(x * 2, tl, m, l, r, v); update(x * 2 + 1, m + 1, tr, l, r, v);
    TR[x].val = C::merge(TR[x * 2].val, TR[x * 2 + 1].val);
  }
  Data query(int x, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return C::qdef();
    if (l <= tl && tr <= r) return TR[x].val;
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    return C::merge(query(x * 2, tl, m, l, r),
                   query(x * 2 + 1, m + 1, tr, l, r));
  }
  template <class F>
  int bsearchPrefix(int x, int tl, int tr, int l, int r, Data &agg, F f) {
    if (r < tl || tr < l) return r + 1;
    if (tl != tr) propagate(x, tl, tr);
    if (l <= tl && tr <= r) {
      Data v = C::merge(agg, TR[x].val); if (!f(v)) { agg = v; return r + 1; }
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
      Data v = C::merge(agg, TR[x].val); if (!f(v)) { agg = v; return l - 1; }
    }
    if (tl == tr) return tl;
    int m = tl + (tr - tl) / 2;
    int ret = bsearchSuffix(x * 2 + 1, m + 1, tr, l, r, agg, f);
    return l <= ret ? ret : bsearchSuffix(x * 2, tl, m, l, r, agg, f);
  }
  template <class F> SegmentTreeBeats(int N, F f)
      : N(N), TR(N == 0 ? 0 : 1 << __lg(N * 4 - 1)) {
    if (N > 0) { build(1, 0, N - 1, f); }
  }
  template <class It> SegmentTreeBeats(It st, It en)
      : SegmentTreeBeats(en - st, [&] { return *st++; }) {}
  SegmentTreeBeats(int N, const Data &vdef)
      : SegmentTreeBeats(N, [&] { return vdef; }) {}
  void update(int l, int r, const Arg &v) { update(1, 0, N - 1, l, r, v); }
  Data query(int l, int r) { return query(1, 0, N - 1, l, r); }
  template <class F> int bsearchPrefix(int l, int r, F f) {
    Data agg = C::qdef(); return bsearchPrefix(1, 0, N - 1, l, r, agg, f);
  }
  template <class F> int bsearchSuffix(int l, int r, F f) {
    Data agg = C::qdef(); return bsearchSuffix(1, 0, N - 1, l, r, agg, f);
  }
};
