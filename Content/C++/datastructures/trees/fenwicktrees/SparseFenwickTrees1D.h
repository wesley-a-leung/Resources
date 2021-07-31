#pragma once
#include <bits/stdc++.h>
#include "../../PolicyBasedDataStructures.h"
using namespace std;
using namespace __gnu_pbds;

// A collection of sparse fenwick trees implemented in various methods
// If bsearch is not required and online is enforced, then SqrtBufferSimple and
//   SqrtBuffer should be used instead
// If updates only involve values 1 and -1, and online is enforced,
//   then RootArray should be used instead
// In general, offline fenwick tree is faster than SqrtBufferSimple,
//   SqrtBuffer, and RootArray, which are faster than the pbds implementation

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries over a cumulative function or functor,
//   such as sum, max, and min, in 1 dimension
// All update indices must be known beforehand
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   IndexType: the type of the index of the array
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutatitve
// Constructor Arguments:
//   N: the size of the array
//   updateInds: a vector of IndexType containing the indices for each update
//   qdef: the identity element of the operation
//   op: an instance of the Op struct
// Functions:
//   update(i, v): modifies the value A[i] to op(A[i], v), i must be an element
//     in updateInds
//   query(r): queries the cumulative value of the range [0, r]
//   query(l, r, inv): queries the cumulative value of the range [l, r], where
//     inv is the inverse of op (minus<T>() by default)
//   bsearch(v, cmp): returns the first index where cmp(op(A[0..i]), v)
//     returns false, or N if no such index exists, assumes A is sorted by cmp
// In practice, has a small constant
// Time Complexity:
//   constructor: O(Q log U) for U updates
//   update, query, bsearch: O(log U) for U updates
// Memory Complexity: O(U) for U updates
// Tested:
//   https://codeforces.com/contest/903/problem/D
//   https://dmoj.ca/problem/cco10p3
//   https://atcoder.jp/contests/dp/tasks/dp_q
template <class T, class IndexType, class Op = plus<T>>
struct OfflineSparseFenwickTree1D {
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  IndexType N; vector<T> BIT; vector<IndexType> inds; T qdef; Op op;
  int getInd(IndexType i) {
    return std::upper_bound(inds.begin(), inds.end(), i) - inds.begin();
  }
  OfflineSparseFenwickTree1D(IndexType N, const vector<IndexType> &updateInds,
                             T qdef = T(), Op op = Op())
      : N(N), inds(updateInds), qdef(qdef), op(op) {
    sort(inds.begin(), inds.end());
    inds.erase(unique(inds.begin(), inds.end()), inds.end());
    BIT.assign(int(inds.size()) + 1, qdef);
  }
  void update(IndexType i, T v) {
    for (int x = getInd(i); x <= int(inds.size()); x += x & -x)
      BIT[x] = op(BIT[x], v);
  }
  T query(IndexType r) {
    T ret = qdef;
    for (int x = getInd(r); x > 0; x -= x & -x) ret = op(ret, BIT[x]);
    return ret;
  }
  template <class Inv = minus<T>>
  T query(IndexType l, IndexType r, Inv inv = Inv()) {
    return inv(query(r), query(l - 1));
  }
  template <class F> IndexType bsearch(T v, F cmp) {
    T agg = qdef; int ind = 0;
    for (int j = __lg(int(inds.size()) + 1); j >= 0; j--) {
      int i = ind + (1 << j);
      if (i < int(inds.size()) && cmp(op(agg, BIT[i]), v))
        agg = op(agg, BIT[ind = i]);
    }
    return ind == int(inds.size()) ? N : inds[ind];
  }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries over a cumulative function or functor,
//   such as sum, max, and min, in 1 dimension using pbds hash_table
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   IndexType: the type of the index of the array
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutatitve
// Constructor Arguments:
//   N: the size of the array
//   qdef: the identity element of the operation
//   op: an instance of the Op struct
// Functions:
//   update(i, v): modifies the value A[i] to op(A[i], v)
//   query(r): queries the cumulative value of the range [0, r]
//   query(l, r, inv): queries the cumulative value of the range [l, r], where
//     inv is the inverse of op (minus<T>() by default)
//   bsearch(v, cmp): returns the first index where cmp(op(A[0..i]), v)
//     returns false, or N if no such index exists
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(1)
//   update, query, bsearch: O(log N) on average
// Memory Complexity: O(U log N) for U updates
// Tested:
//   https://codeforces.com/contest/903/problem/D
//   https://dmoj.ca/problem/cco10p3
//   https://atcoder.jp/contests/dp/tasks/dp_q
template <class T, class IndexType, class Container = hashmap<IndexType, T>,
          class Op = plus<T>>
struct SparseFenwickTree1D {
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  IndexType N; Container BIT; T qdef; Op op;
  SparseFenwickTree1D(IndexType N, T qdef = T(), Op op = Op())
      : N(N), qdef(qdef), op(op) {}
  void update(IndexType i, T v) {
    for (i++; i <= N; i += i & -i) BIT[i] = op(BIT[i], v);
  }
  T query(IndexType r) {
    T ret = qdef; for (r++; r > 0; r -= r & -r) {
      auto it = BIT.find(r); if (it != BIT.end()) ret = op(ret, it->second);
    }
    return ret;
  }
  template <class Inv = minus<T>>
  T query(IndexType l, IndexType r, Inv inv = Inv()) {
    return inv(query(r), query(l - 1));
  }
  template <class F> IndexType bsearch(T v, F cmp) {
    T agg = qdef; IndexType ind = 0;
    for (IndexType j = __lg(N + 1); j >= 0; j--) {
      IndexType i = ind + (1 << j); if (i <= N) {
        auto it = BIT.find(i); T val = it == BIT.end() ? 0 : it->second;
        if (cmp(op(agg, val), v)) { agg = op(agg, val); ind = i; }
      }
    }
    return ind;
  }
};
