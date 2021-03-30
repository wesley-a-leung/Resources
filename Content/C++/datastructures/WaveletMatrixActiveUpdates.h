#pragma once
#include <bits/stdc++.h>
#include "BitPrefixSumArray.h"
#include "trees/fenwicktrees/BitFenwickTree.h"
using namespace std;

// Wavelet Matrix supporting rank and select operations for a subarray where
//   elements can be updated to be active or not
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of the element of the array
//   Cmp: the comparator to compare two elements
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   A: a vector of type T containing the elements of the array
//   isActive: a vector of booleans indicating whether each element is
//     initially active or not
// Functions:
//   update(i, activeStatus): updates the status of the ith element to
//     activeStatus (active if true, inactive if false)
//   rank(l, r, k): returns the number of active elements less than k (using
//     the comparator) in the range [l, r]
//   count(l, r, lo, hi) returns the number of active elements not less than
//     lo and not greater than hi (using the comparator) in the range [l, r]
//   select(l, r, k): selects the kth active element sorted by the comparator
//     if the range [l, r] was sorted
// In practice, has a small constant, faster than using a
//   2D Sparse Fenwick Tree
// Time Complexity:
//   constructor: O(N log N)
//   update, rank, count, select: O(log(N) log(N / 64))
// Memory Complexity: O((N log N) / 64)
// Tested:
//   https://dmoj.ca/problem/dmopc19c7p5 (rank/count)
template <class T, class Cmp = less<T>> struct WaveletMatrixActiveUpdates {
  int N, H; vector<int> mid; vector<BitPrefixSumArray> B;
  vector<BitFenwickTree> active; vector<T> S;
  WaveletMatrixActiveUpdates(const vector<T> &A, const vector<bool> isActive)
      : N(A.size()), H(N == 0 ? 0 : __lg(N) + 1), mid(H),
        B(H, BitPrefixSumArray(N)), active(H, BitFenwickTree(N)), S(A) {
    vector<T> temp = S; sort(S.begin(), S.end(), Cmp());
    vector<int> C(N), ind(N); for (int i = 0; i < N; i++)
      C[i] = lower_bound(S.begin(), S.end(), temp[i], Cmp()) - S.begin();
    iota(ind.begin(), ind.end(), 0); for (int h = H - 1; h >= 0; h--) {
      int ph = 1 << h; for (int i = 0; i < N; i++) if (C[ind[i]] <= ph - 1) {
        B[h].set(i, 1); active[h].set(i, isActive[ind[i]]);
      }
      mid[h] = stable_partition(ind.begin(), ind.end(), [&] (int i) {
                                  return C[i] <= ph - 1;
                                }) - ind.begin();
      B[h].build(); active[h].build();
      for (int i = mid[h]; i < N; i++) C[ind[i]] -= ph;
    }
  }
  void update(int i, bool activeStatus) {
    for (int h = H - 1; h >= 0; h--) {
      if (B[h].get(i)) {
        active[h].update(i, activeStatus); i = B[h].query(i - 1);
      } else i += mid[h] - B[h].query(i - 1);
    }
  }
  template <class F> int cnt(int l, int r, const T &v, F f) {
    int ret = 0, cur = 0; for (int h = H - 1; h >= 0; h--) {
      int ph = 1 << h, ql = B[h].query(l - 1), qr = B[h].query(r);
      int al = active[h].query(l - 1), ar = active[h].query(r);
      if (cur + ph - 1 >= N || f(v, S[cur + ph - 1])) { l = ql; r = qr - 1; }
      else { cur += ph; ret += ar - al; l += mid[h] - ql; r += mid[h] - qr; }
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
      int al = active[h].query(l - 1), ar = active[h].query(r);
      if (k < ar - al) { l = ql; r = qr - 1; }
      else { cur += 1 << h; k -= ar - al; l += mid[h] - ql; r += mid[h] - qr; }
    }
    return S[cur];
  }
};
