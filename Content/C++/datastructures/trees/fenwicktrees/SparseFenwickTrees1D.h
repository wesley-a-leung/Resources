#pragma once
#include <bits/stdc++.h>
#include "../../PolicyBasedDataStructures.h"
using namespace std;
using namespace __gnu_pbds;

// A collection of sparse fenwick trees implemented in various methods
// If lower_bound and upper_bound operations are not required and online
//   is enforced, then SqrtBufferSimple and SqrtBuffer should be used instead
// If updates only involve values 1 and -1, and online is enforced,
//   then RootArray should be used instead
// In general, offline fenwick tree is faster than SqrtBufferSimple,
//   SqrtBuffer, and RootArray, which are faster than the pbds implementation

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries in 1 dimension
// All update indices must be known beforehand
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   IndexType: the type of the index of the array
// Constructor Arguments:
//   N: the size of the array
//   updateInds: a vector of IndexType containing the indices for each update
// Functions:
//   update(i, v): add v to the value at index i
//   query(r): queries the range [0, r]
//   query(l, r): queries the range [l, r]
//   bsearch(v, cmp): returns the first index where cmp(sum(A[0..i]), v)
//     returns false, or N if no such index exists
//   lower_bound(v): returns the first index where sum(A[0..i]) >= v, assumes
//     cmp(sum(A[0..i + 1]), sum(A[0..i])) returns false
//   upper_bound(v): returns the first index where sum(A[0..i]) > v, assumes
//     cmp(sum(A[0..i + 1]), sum(A[0..i])) returns false
// In practice, has a small constant
// Time Complexity:
//   constructor: O(Q log Q) for Q updates
//   update, query, bsearch, lower_bound, upper_bound: O(log Q) for Q updates
// Memory Complexity: O(Q log Q) for Q updates
// Tested:
//   https://dmoj.ca/problem/ccc05s5
//   https://dmoj.ca/problem/cco10p3
template <class T, class IndexType> struct OfflineSparseFenwickTree1D {
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  IndexType N; vector<T> BIT; vector<IndexType> inds;
  int getInd(IndexType i) {
    return std::upper_bound(inds.begin(), inds.end(), i) - inds.begin();
  }
  OfflineSparseFenwickTree1D(IndexType N, const vector<IndexType> &updateInds)
      : N(N), inds(updateInds) {
    sort(inds.begin(), inds.end());
    inds.erase(unique(inds.begin(), inds.end()), inds.end());
    BIT.assign(int(inds.size()) + 1, T());
  }
  void update(IndexType i, T v) {
    for (int x = getInd(i); x <= int(inds.size()); x += x & -x) BIT[x] += v;
  }
  T query(IndexType r) {
    T ret = T(); for (int x = getInd(r); x > 0; x -= x & -x) ret += BIT[x];
    return ret;
  }
  T query(IndexType l, IndexType r) { return query(r) - query(l - 1); }
  template <class F> IndexType bsearch(T v, F cmp) {
    T sum = T(); int ind = 0;
    for (int j = __lg(int(inds.size()) + 1); j >= 0; j--) {
      int i = ind + (1 << j);
      if (i < int(inds.size()) && cmp(sum + BIT[i], v)) sum += BIT[ind = i];
    }
    return ind == int(inds.size()) ? N : inds[ind];
  }
  IndexType lower_bound(T v) { return bsearch(v, less<T>()); }
  IndexType upper_bound(T v) { return bsearch(v, less_equal<T>()); }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries in 1 dimension using pbds hash_table
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   IndexType: the type of the index of the array
// Constructor Arguments:
//   N: the size of the array
// Functions:
//   update(i, v): add v to the value at index i
//   query(r): queries the range [0, r]
//   query(l, r): queries the range [l, r]
//   bsearch(v, cmp): returns the first index where cmp(sum(A[0..i]), v)
//     returns false, or N if no such index exists
//   lower_bound(v): returns the first index where sum(A[0..i]) >= v, assumes
//     A is sorted by cmp
//   upper_bound(v): returns the first index where sum(A[0..i]) > v, assumes
//     A is sorted by cmp
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(1)
//   update, query, bsearch, lower_bound, upper_bound: O(log N) on average
// Memory Complexity: O(Q log N) for Q updates
// Tested:
//   https://dmoj.ca/problem/ds4
//   https://dmoj.ca/problem/cco10p3
template <class T, class IndexType, class Container = hashmap<IndexType, T>>
struct SparseFenwickTree1D {
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  IndexType N; Container BIT; SparseFenwickTree1D(IndexType N) : N(N) {}
  void update(IndexType i, T v) { for (i++; i <= N; i += i & -i) BIT[i] += v; }
  T query(IndexType r) {
    T ret = T(); for (r++; r > 0; r -= r & -r) {
      auto it = BIT.find(r); if (it != BIT.end()) ret += it->second;
    }
    return ret;
  }
  T query(IndexType l, IndexType r) { return query(r) - query(l - 1); }
  template <class F> IndexType bsearch(T v, F cmp) {
    T sum = T(); IndexType ind = 0;
    for (IndexType j = __lg(N + 1); j >= 0; j--) {
      IndexType i = ind + (1 << j); if (i <= N) {
        auto it = BIT.find(i); T add = it == BIT.end() ? 0 : it->second;
        if (cmp(sum + add, v)) { sum += add; ind = i; }
      }
    }
    return ind;
  }
  IndexType lower_bound(T v) { return bsearch(v, less<T>()); }
  IndexType upper_bound(T v) { return bsearch(v, less_equal<T>()); }
};
