#pragma once
#include <bits/stdc++.h>
using namespace std;

// Segment Tree supporting range updates with updates in the form of
//   adding m + b, 2m + b, 3m + b, ... to the interval [l, r],
//   and range sum queries
// Indices are 0-indexed and ranges are inclusive
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
  const Pair ZERO = Pair(0, 0); int N; vector<Pair> TR; vector<Pair> LZ;
  T sumTo(T k) { return k * (k + 1) / 2; }
  Pair sumBet(T l, T r) { return Pair(r - l + 1, sumTo(r) - sumTo(l - 1)); }
  void propagate(int x, int tl, int tr) {
    if (LZ[x] != ZERO) {
      int m = tl + (tr - tl) / 2, rc = x + (m - tl + 1) * 2;
      TR[x + 1] = add(TR[x + 1], mul(LZ[x], sumBet(tl, m)));
      LZ[x + 1] = add(LZ[x + 1], LZ[x]);
      TR[rc] = add(TR[rc], mul(LZ[x], sumBet(m + 1, tr)));
      LZ[rc] = add(LZ[rc], LZ[x]); LZ[x] = ZERO;
    }
  }
  template <class F> void build(int x, int tl, int tr, F a) {
    if (tl == tr) { TR[x] = a(tl); return; }
    int m = tl + (tr - tl) / 2, rc = x + (m - tl + 1) * 2;
    build(x + 1, tl, m, a); build(rc, m + 1, tr, a);
    TR[x] = add(TR[x + 1], TR[rc]);
  }
  void update(int x, int tl, int tr, int l, int r, const Pair &v) {
    if (l <= tl && tr <= r) {
      TR[x] = add(TR[x], mul(v, sumBet(tl, tr))); LZ[x] = add(LZ[x], v);
      return;
    }
    propagate(x, tl, tr);
    int m = tl + (tr - tl) / 2, rc = x + (m - tl + 1) * 2;
    if (tl <= r && l <= m) update(x + 1, tl, m, l, r, v);
    if (m + 1 <= r && l <= tr) update(rc, m + 1, tr, l, r, v);
    TR[x] = add(TR[x + 1], TR[rc]);
  }
  Pair query(int x, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return ZERO;
    if (l <= tl && tr <= r) return TR[x];
    propagate(x, tl, tr);
    int m = tl + (tr - tl) / 2, rc = x + (m - tl + 1) * 2;
    return add(query(x + 1, tl, m, l, r), query(rc, m + 1, tr, l, r));
  }
  template <class It> SegmentTreeLinear(It st, It en)
      : N(en - st), TR(N * 2 - 1, ZERO), LZ(N * 2 - 1, ZERO) {
    build(0, 0, N - 1, [&] (int i) { return Pair(*(st + i), 0); });
  }
  SegmentTreeLinear(int N, const T &vdef)
      : N(N), TR(N * 2 - 1, ZERO), LZ(N * 2 - 1, ZERO) {
    build(0, 0, N - 1, [&] (int i) { return Pair(vdef, 0); });
  }
  void update(int l, int r, T m, T b) {
    update(0, 0, N - 1, l, r, Pair((1 - l) * m + b, m));
  }
  T query(int l, int r) {
    Pair q = query(0, 0, N - 1, l, r); return q.first + q.second;
  }
};
