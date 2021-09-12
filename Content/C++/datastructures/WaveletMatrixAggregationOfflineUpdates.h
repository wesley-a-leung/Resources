#pragma once
#include <bits/stdc++.h>
#include "WaveletMatrixAggregation.h"
using namespace std;

// Wavelet Matrix supporting 2D aggregation queries where the data can change,
//   and the indices and the keys that are updated are known beforehand
// Indices are 0-indexed and ranges are inclusive
// Queries bounded by 3 indices require a commutative operation, while queries
//   bounded by 4 indices also requires the data to be invertible
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of the element of the array
//   R: struct supporting point updates and range queries on indices
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value
//       static merge(l, r): merges the datas l and r
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data
//       static invData(v): returns the inverse of v of type Data
//       constructor(A): takes a vector A of type Data with the initial
//         value of each index
//       update(i, val): updates the index i with the value val
//       query(l, r): queries the range [l, r]
//     Sample Struct: supporting point increments updates and range sum queries
//       struct R {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         static Data applyLazy(const Data &l, const Lazy &r) {
//           return l + r;
//         }
//         static Data invData(const Data &v) { return -v; }
//         FenwickTree1D<Data> FT;
//         R(vector<Data> A) : FT(move(A)) {}
//         void update(int i, const Lazy &val) { FT.update(i, val); }
//         Data query(int l, int r) { return FT.query(l, r); }
//       };
//   Cmp: the comparator to compare two elements of type T
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   A: a vector of type T containing the initial keys of the array
//   D: a vector of type R::Data containing the initial data of the array
//   updates: a vector of vectors of type T containing the sequence of updates
//     to each index in the order they are updated
//   cmp: an instance of the Cmp struct
// Functions:
//   updateToNextKey(i): updates the ith key to the next key in the update
//     sequence
//   update(i, v): updates the ith data with the lazy value v
//   query(l, r, hi): returns the aggregate value of the data associated with
//     all elements with a key less than or equal to hi (using the comparator)
//     in the range [l, r]
//   query(l, r, lo, hi): returns the aggregate value of the data associated
//     with all elements with a key of not less than lo and not greater than hi
//     (using the comparator) in the range [l, r]
//   bsearch(l, r, f): over all keys in the array, finds the first key k such
//     that query(l, r, k) returns true
// In practice, has a small constant, faster than using an
//   Offline or Online 2D Sparse Fenwick Tree
// Time Complexity:
//   constructor: O((N + K + C) log N + K) where C is the time complexity of
//     R's constructor for K total updates
//   updateToNextKey: O(U log (N + K)) where U is the time complexity of R's
//     update function for K total updates
//   update: O(U log (N + K)) where U is the time complexity of R's update
//     function for K total updates
//   query, bsearch: O(Q log N) where Q is the time complexity of
//     R's query function for K total updates
// Memory Complexity: O(N + K + ((N + K) log (N + K)) / 64 + M log (N + K))
//   where M is the memory complexity of R for K total updates
// Tested:
//   https://dmoj.ca/problem/oly19practice4
template <class T, class R, class Cmp = less<T>>
struct WaveletMatrixAggregationOfflineUpdates {
  using Data = typename R::Data; using Lazy = typename R::Lazy;
  vector<int> cur; vector<Data> D; WaveletMatrixAggregation<T, R, Cmp> wm;
  WaveletMatrixAggregation<T, R, Cmp> init(
      const vector<T> &A, const vector<Data> &D,
      const vector<vector<T>> &updates, Cmp cmp) {
    assert(A.size() == D.size()); assert(A.size() == updates.size());
    int N = A.size();
    for (int i = 0; i < N; i++) cur[i + 1] = updates[i].size() + 1;
    partial_sum(cur.begin(), cur.end(), cur.begin());
    vector<T> ret; ret.reserve(cur[N]); for (int i = 0; i < N; i++) {
      ret.push_back(A[i]);
      ret.insert(ret.end(), updates[i].begin(), updates[i].end());
    }
    vector<Data> temp(ret.size(), R::qdef());
    for (int i = 0; i < N; i++) temp[cur[i]] = D[i];
    return WaveletMatrixAggregation<T, R, Cmp>(move(ret), temp, cmp);
  }
  WaveletMatrixAggregationOfflineUpdates(
      const vector<T> &A, const vector<Data> &D,
      const vector<vector<T>> &updates, Cmp cmp = Cmp())
      : cur(A.size() + 1, 0), D(D), wm(init(A, D, updates, cmp)) {}
  void updateToNextKey(int i) {
    wm.update(cur[i], R::qdef()); wm.update(++cur[i], D[i]);
  }
  void update(int i, const Lazy &v) {
    D[i] = R::applyLazy(D[i], v); wm.update(cur[i], v);
  }
  Data query(int l, int r, const T &hi) {
    return wm.query(cur[l], cur[r], hi);
  }
  Data query(int l, int r, const T &lo, const T &hi) {
    return wm.query(cur[l], cur[r], lo, hi);
  }
  template <class F> pair<bool, T *> bsearch(int l, int r, F f) {
    return wm.bsearch(cur[l], cur[r], f);
  }
};
