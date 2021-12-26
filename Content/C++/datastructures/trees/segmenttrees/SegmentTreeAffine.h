#pragma once
#include <bits/stdc++.h>
using namespace std;

// Segment Tree supporting range updates with updates in the form of
//   adding m + b, 2m + b, 3m + b, ... to the interval [l, r],
//   and range sum queries
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of the element of the array
// Constructor Arguments:
//   N: the size of the array
//   A: a vector of type T
// Functions:
//   update(l, r, m, b): update the range [l, r] with
//     m + b, 2m + b, 3m + b, ...
//   query(l, r): returns the sum of elements in the range [l, r]
// In practice, has a small constant, but slower than FenwickTreeAffine
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/acc3p4
template <class T> struct SegmentTreeAffine {
  using Pair = pair<T, T>;
  Pair add(const Pair &l, const Pair &r) {
    return Pair(l.first + r.first, l.second + r.second);
  }
  Pair mul(const Pair &l, const Pair &r) {
    return Pair(l.first * r.first, l.second * r.second);
  }
  int N; vector<Pair> TR, LZ;
  T sumTo(T k) { return k * (k + 1) / 2; }
  Pair sumBet(T l, T r) { return Pair(r - l + 1, sumTo(r) - sumTo(l - 1)); }
  void apply(int x, int tl, int tr, const Pair &v) {
    TR[x] = add(TR[x], mul(v, sumBet(tl, tr)));
    if (x < int(LZ.size())) LZ[x] = add(LZ[x], v);
  }
  void propagate(int x, int tl, int tr) {
    if (LZ[x] != Pair()) {
      int m = tl + (tr - tl) / 2; apply(x * 2, tl, m, LZ[x]);
      apply(x * 2 + 1, m + 1, tr, LZ[x]); LZ[x] = Pair();
    }
  }
  void build(const vector<T> &A, int x, int tl, int tr) {
    if (tl == tr) { TR[x] = Pair(A[tl], T()); return; }
    int m = tl + (tr - tl) / 2; build(A, x * 2, tl, m);
    build(A, x * 2 + 1, m + 1, tr); TR[x] = add(TR[x * 2], TR[x * 2 + 1]);
  }
  void update(int x, int tl, int tr, int l, int r, const Pair &v) {
    if (l <= tl && tr <= r) { apply(x, tl, tr, v); return; }
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    if (tl <= r && l <= m) update(x * 2, tl, m, l, r, v);
    if (m + 1 <= r && l <= tr) update(x * 2 + 1, m + 1, tr, l, r, v);
    TR[x] = add(TR[x * 2], TR[x * 2 + 1]);
  }
  Pair query(int x, int tl, int tr, int l, int r) {
    if (r < tl || tr < l) return Pair();
    if (l <= tl && tr <= r) return TR[x];
    propagate(x, tl, tr); int m = tl + (tr - tl) / 2;
    return add(query(x * 2, tl, m, l, r), query(x * 2 + 1, m + 1, tr, l, r));
  }
  SegmentTreeAffine(int N)
      : N(N), TR(N == 0 ? 0 : 1 << __lg(N * 4 - 1)),
        LZ(N == 0 ? 0 : 1 << __lg(N * 2 - 1)) {}
  SegmentTreeAffine(const vector<T> &A) : SegmentTreeAffine(A.size()) {
    if (N > 0) build(A, 1, 0, N - 1);
  }
  void update(int l, int r, T m, T b) {
    update(1, 0, N - 1, l, r, Pair((1 - l) * m + b, m));
  }
  T query(int l, int r) {
    Pair q = query(1, 0, N - 1, l, r); return q.first + q.second;
  }
};
