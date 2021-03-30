#pragma once
#include <bits/stdc++.h>
#include "WaveletMatrixActiveUpdates.h"
using namespace std;

// Wavelet Matrix supporting rank and select operations for a subarray where
//   the indices and values of elements that are updated are known beforehand
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of the element of the array
//   Cmp: the comparator to compare two elements
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   A: a vector of type T containing the initial elements of the array
//   updates: a vector of vectors of type T containing the sequence of updates
//     to each index in the order they are updated
// Functions:
//   updateToNextVal(i): updates the value of index i to the next element
//     in that index's sequence of updates
//   rank(l, r, k): returns the number of elements less than k (using
//     the comparator) in the range [l, r] in the current array
//   count(l, r, lo, hi) returns the number of elements not less than
//     lo and not greater than hi (using the comparator) in the range [l, r]
//     in the current array
//   select(l, r, k): selects the kth element sorted by the comparator
//     if the range [l, r] was sorted in the current array
// In practice, has a small constant, faster than using an
//   Offline or Online 2D Sparse Fenwick Tree
// Time Complexity:
//   constructor: O((N + U) log (N + U)) for U total updates
//   update, rank, count, select: O(log(N + U) log((N + U) / 64)) for
//     U total updates
// Memory Complexity: O(((N + U) log (N + U)) / 64) for U total updates
// Tested:
//   https://www.spoj.com/problems/KQUERY2/ (rank/count)
template <class T, class Cmp = less<T>> struct WaveletMatrixOfflineUpdates {
  vector<int> cur; WaveletMatrixActiveUpdates<T, Cmp> wm;
  WaveletMatrixActiveUpdates<T, Cmp> init(const vector<T> &A,
                                          const vector<vector<T>> &updates) {
    assert(A.size() == updates.size()); int N = A.size();
    for (int i = 0; i < N; i++) cur[i + 1] = 1 + int(updates[i].size());
    partial_sum(cur.begin(), cur.end(), cur.begin());
    vector<T> ret; ret.reserve(cur[N]); for (int i = 0; i < N; i++) {
      ret.push_back(A[i]);
      ret.insert(ret.end(), updates[i].begin(), updates[i].end());
    }
    vector<bool> isActive(ret.size(), false);
    for (int i = 0; i < N; i++) isActive[cur[i]] = true;
    return WaveletMatrixActiveUpdates<T, Cmp>(ret, isActive);
  }
  WaveletMatrixOfflineUpdates(const vector<T> &A,
                              const vector<vector<T>> &updates)
      : cur(A.size() + 1, 0), wm(init(A, updates)) {}
  void updateToNextVal(int i) { wm.update(cur[i], 0); wm.update(++cur[i], 1); }
  int rank(int l, int r, const T &v) { return wm.rank(cur[l], cur[r], v); }
  int count(int l, int r, const T &lo, const T &hi) {
    return wm.count(cur[l], cur[r], lo, hi);
  }
  T select(int l, int r, int k) { return wm.select(cur[l], cur[r], k); }
};
