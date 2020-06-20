#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fischer Heun Structure supporting range maximum queries on a static array
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// queryInd: returns the index of the maximum element in the subarray [l, r],
//   breaking ties by selecting the first such index
// query: returns the maximum element in the subarray [l, r]
// In practice, the constructor has a moderate constant and is significantly
//   faster than sparse table's constructor, query has a moderate constant and
//   is slightly slower than sparse table's query
// Time Complexity:
//   constructor: O(N / B log (N / B)), where B is the number of bits in mask_t
//   query: O(1)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ncco3d2p1
//   https://www.spoj.com/problems/RMQSQ/
//   https://judge.yosupo.jp/problem/staticrmq
template <class T, class Comparator = less<T>, class mask_t = uint32_t>
struct FischerHeunStructure {
  static_assert(is_integral<mask_t>::value, "mask_t must be integral");
  static_assert(is_unsigned<mask_t>::value, "mask_t must be unsigned");
  static constexpr int B = __lg(numeric_limits<mask_t>::max()) + 1;
  int N, M; vector<T> A; vector<mask_t> mask; vector<vector<int>> ST;
  int cmpInd(int i, int j) { return Comparator()(A[i], A[j]) ? j : i; }
  int small(int r, int sz) {
    return r - __lg(sz == B ? mask[r] : mask[r] & ((mask_t(1) << sz) - 1));
  }
  template <class F> FischerHeunStructure(int N, F f)
      : N(N), M(N / B), mask(N), ST(M == 0 ? 1 : __lg(M) + 1, vector<int>(M)) {
    A.reserve(N); mask_t k = 0; for (int i = 0; i < N; mask[i++] = k |= 1)
      for (A.push_back(f()), k <<= 1; k && cmpInd(i - __lg(k & -k), i) == i;)
        k ^= k & -k;
    for (int i = 0; i < M; i++) ST[0][i] = small(B * (i + 1) - 1, B);
    for (int i = 0; i < int(ST.size()) - 1; i++) for (int j = 0; j < M; j++)
      ST[i + 1][j] = cmpInd(ST[i][j], ST[i][min(j + (1 << i), M - 1)]);
  }
  template <class It> FischerHeunStructure(It st, It en)
      : FischerHeunStructure(en - st, [&] { return *st++; }) {}
  int queryInd(int l, int r) {
    if (r - l + 1 <= B) return small(r, r - l + 1);
    int ql = small(l + B - 1, B), qr = small(r, B);
    l = l / B + 1; r = r / B - 1; if (l <= r) {
      int i = __lg(r - l + 1);
      ql = cmpInd(ql, cmpInd(ST[i][l], ST[i][r - (1 << i) + 1]));
    }
    return cmpInd(ql, qr);
  }
  T query(int l, int r) { return A[queryInd(l, r)]; }
};
