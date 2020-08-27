#pragma once
#include <bits/stdc++.h>
using namespace std;

// Segment Tree supporting range updates with updates in the form of
//   adding m + b, 2m + b, 3m + b, ... to the interval [l, r],
//   and range sum queries
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of the element of the array
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   vdef: the default value to fill the array with
// Functions:
//   update(l, r, m, b): update the range [l, r] with
//     m + b, 2m + b, 3m + b, ...
//   query(l, r): returns the sum of elements in the range [l, r]
// In practice, has a small constant, but slower than FenwickTreeLinear
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/acc3p4
template <class T> struct SegmentTreeLinear {
  using Pair = pair<T, T>;
  Pair add(const Pair &l, const Pair &r) {
    return Pair(l.first + r.first, l.second + r.second);
  }
  Pair mul(const Pair &l, const Pair &r) {
    return Pair(l.first * r.first, l.second * r.second);
  }
  struct Node { Pair val, lz; }; int N; vector<Node> TR;
  T sumTo(T k) { return k * (k + 1) / 2; }
  Pair sumBet(T l, T r) { return Pair(r - l + 1, sumTo(r) - sumTo(l - 1)); }
  void apply(int x, int tl, int tr, const Pair &v) {
    TR[x].val = add(TR[x].val, mul(v, sumBet(tl, tr)));
    TR[x].lz = add(TR[x].lz, v);
  }
  void propagate(int x, int tl, int tr) {
    if (TR[x].lz != Pair()) {
      int m = tl + (tr - tl) / 2; apply(x * 2, tl, m, TR[x].lz);
      apply(x * 2 + 1, m + 1, tr, TR[x].lz); TR[x].lz = Pair();
    }
  }
  template <class F> void build(int x, int tl, int tr, F &f) {
    if (tl == tr) { TR[x].val = Pair(f(), T()); return; }
    int m = tl + (tr - tl) / 2;
    build(x * 2, tl, m, f); build(x * 2 + 1, m + 1, tr, f);
    TR[x].val = add(TR[x * 2].val, TR[x * 2 + 1].val);
  }
  void update(int x, int tl, int tr, int l, int r, const Pair &v) {
    if (l <= tl && tr <= r) { apply(x, tl, tr, v); return; }
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    if (tl <= r && l <= m) update(x * 2, tl, m, l, r, v);
    if (m + 1 <= r && l <= tr) update(x * 2 + 1, m + 1, tr, l, r, v);
    TR[x].val = add(TR[x * 2].val, TR[x * 2 + 1].val);
  }
  Pair query(int x, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return Pair();
    if (l <= tl && tr <= r) return TR[x].val;
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    return add(query(x * 2, tl, m, l, r), query(x * 2 + 1, m + 1, tr, l, r));
  }
  template <class F> SegmentTreeLinear(int N, F f)
      : N(N), TR(N == 0 ? 0 : 1 << __lg(N * 4 - 1)) {
    if (N > 0) build(1, 0, N - 1, f);
  }
  template <class It> SegmentTreeLinear(It st, It en)
      : SegmentTreeLinear(en - st, [&] { return *st++; }) {}
  SegmentTreeLinear(int N, const T &vdef)
      : SegmentTreeLinear(N, [&] { return vdef; }) {}
  void update(int l, int r, T m, T b) {
    update(1, 0, N - 1, l, r, Pair((1 - l) * m + b, m));
  }
  T query(int l, int r) {
    Pair q = query(1, 0, N - 1, l, r); return q.first + q.second;
  }
};
