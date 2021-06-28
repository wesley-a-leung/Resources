#pragma once
#include <bits/stdc++.h>
#include "BitPrefixSumArray.h"
using namespace std;

// Wavelet Matrix supporting rank and select operations for a subarray
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of the element of the array
//   Cmp: the comparator to compare two elements
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Functions:
//   rank(l, r, k): returns the number of elements less than k (using the
//     comparator) in the range [l, r]
//   count(l, r, lo, hi) returns the number of elements not less than lo and
//     not greater than hi (using the comparator) in the range [l, r]
//   select(l, r, k): returns the kth element sorted by the comparator if the
//     range [l, r] was sorted
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N log N)
//   rank, count, select: O(log N)
// Memory Complexity: O(N + (N log N) / 64)
// Tested:
//   https://www.spoj.com/problems/KQUERY/ (rank/count)
//   https://www.spoj.com/problems/KQUERYO/ (rank/count)
//   https://codeforces.com/contest/1284/problem/D (rank/count)
//   https://www.spoj.com/problems/MKTHNUM/ (select)
//   https://judge.yosupo.jp/problem/range_kth_smallest (select)
template <class T, class Cmp = less<T>> struct WaveletMatrix {
  int N, H; vector<int> mid; vector<BitPrefixSumArray> B; vector<T> S;
  template <class F> WaveletMatrix(int N, F f)
      : N(N), H(N == 0 ? 0 : __lg(N) + 1), mid(H), B(H, BitPrefixSumArray(N)) {
    S.reserve(N); for (int i = 0; i < N; i++) S.push_back(f());
    vector<T> temp = S; sort(S.begin(), S.end(), Cmp()); vector<int> C(N);
    for (int i = 0; i < N; i++)
      C[i] = lower_bound(S.begin(), S.end(), temp[i], Cmp()) - S.begin();
    for (int h = H - 1; h >= 0; h--) {
      int ph = 1 << h; for (int i = 0; i < N; i++) B[h].set(i, C[i] <= ph - 1);
      mid[h] = stable_partition(C.begin(), C.end(), [&] (int v) {
                                  return v <= ph - 1;
                                }) - C.begin();
      B[h].build(); for (int i = mid[h]; i < N; i++) C[i] -= ph;
    }
  }
  template <class It> WaveletMatrix(It st, It en)
      : WaveletMatrix(en - st, [&] { return *st++; }) {}
  template <class F> int cnt(int l, int r, const T &v, F f) {
    int ret = 0, cur = 0; for (int h = H - 1; h >= 0; h--) {
      int ph = 1 << h, ql = B[h].query(l - 1), qr = B[h].query(r);
      if (cur + ph - 1 >= N || f(v, S[cur + ph - 1])) { l = ql; r = qr - 1; }
      else { cur += ph; ret += qr - ql; l += mid[h] - ql; r += mid[h] - qr; }
    }
    return ret;
  }
  int rank(int l, int r, const T &v) {
    return cnt(l, r, v, [&] (const T &a, const T &b) { return !Cmp()(b, a); });
  }
  int count(int l, int r, const T &lo, const T &hi) {
    return cnt(l, r, hi, [&] (const T &a, const T &b) { return Cmp()(a, b); })
        - cnt(l, r, lo, [&] (const T &a, const T &b) { return !Cmp()(b, a); });
  }
  T select(int l, int r, int k) {
    int cur = 0; for (int h = H - 1; h >= 0; h--) {
      int ql = B[h].query(l - 1), qr = B[h].query(r);
      if (k < qr - ql) { l = ql; r = qr - 1; }
      else { cur += 1 << h; k -= qr - ql; l += mid[h] - ql; r += mid[h] - qr; }
    }
    return S[cur];
  }
};
