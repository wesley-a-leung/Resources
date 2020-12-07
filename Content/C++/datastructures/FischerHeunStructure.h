#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fischer Heun Structure supporting range maximum queries on a static array
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of each element
//   Cmp: the comparator to compare two values,
//       convention is same as priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
//   mask_t: the type to store a bitmas, should have around log(N) bits
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   cmp: an instance of the Cmp struct
// Functions:
//   queryInd(l, r): returns the index of the maximum element in
//     the subarray [l, r], breaking ties by selecting the first such index
//   query(l, r): returns the maximum element (based on the comparator) in
//     the subarray [l, r]
// In practice, the constructor has a moderate constant and is significantly
//   faster than sparse table's constructor, query has a moderate constant and
//   is slightly slower than sparse table's query
// Time Complexity:
//   constructor: O(N / B log (N / B)), where B is the number of bits in mask_t
//   queryInd, query: O(1) assuming bitshift for mask_t is O(1)
// Memory Complexity: O(N + N / B log (N / B)) assuming mask_t is O(1) memory
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/ncco3d2p1
//   https://www.spoj.com/problems/RMQSQ/
//   https://judge.yosupo.jp/problem/staticrmq
template <class T, class Cmp = less<T>, class mask_t = uint32_t>
struct FischerHeunStructure {
  static_assert(is_integral<mask_t>::value, "mask_t must be integral");
  static_assert(is_unsigned<mask_t>::value, "mask_t must be unsigned");
  static constexpr const int B = __lg(numeric_limits<mask_t>::max()) + 1;
  int N, M; vector<T> A; vector<mask_t> mask; vector<vector<int>> ST; Cmp cmp;
  int cmpInd(int i, int j) { return cmp(A[i], A[j]) ? j : i; }
  int small(int r, int sz = B) {
    return r - __lg(sz == B ? mask[r] : mask[r] & ((mask_t(1) << sz) - 1));
  }
  template <class F> FischerHeunStructure(int N, F f, Cmp cmp = Cmp())
      : N(N), M(N / B), mask(N), ST(M == 0 ? 0 : __lg(M) + 1, vector<int>(M)),
        cmp(cmp) {
    A.reserve(N); mask_t k = 0; for (int i = 0; i < N; mask[i++] = k |= 1)
      for (A.push_back(f()), k <<= 1; k && cmpInd(i - __lg(k & -k), i) == i;)
        k ^= k & -k;
    for (int i = 0; i < M; i++) ST[0][i] = small(B * (i + 1) - 1);
    for (int i = 0; i < int(ST.size()) - 1; i++) for (int j = 0; j < M; j++)
      ST[i + 1][j] = cmpInd(ST[i][j], ST[i][min(j + (1 << i), M - 1)]);
  }
  template <class It> FischerHeunStructure(It st, It en, Cmp cmp = Cmp())
      : FischerHeunStructure(en - st, [&] { return *st++; }, cmp) {}
  int queryInd(int l, int r) {
    if (r - l + 1 <= B) return small(r, r - l + 1);
    int ql = small(l + B - 1), qr = small(r); l = l / B + 1; r = r / B - 1;
    if (l <= r) {
      int i = __lg(r - l + 1);
      ql = cmpInd(ql, cmpInd(ST[i][l], ST[i][r - (1 << i) + 1]));
    }
    return cmpInd(ql, qr);
  }
  T query(int l, int r) { return A[queryInd(l, r)]; }
};
