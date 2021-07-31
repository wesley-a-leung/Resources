#pragma once
#include <bits/stdc++.h>
#include "../../PolicyBasedDataStructures.h"
#include "../../sqrt/SqrtBufferSimple.h"
#include "../../sqrt/SqrtBuffer.h"
using namespace std;
using namespace __gnu_pbds;

// A collection of sparse fenwick trees implemented in various methods
// Some are only sparse in 1 dimension, others are sparse in 2 dimensions
// Certain implementations only allow for increments and decrements of 1
// In general, offline fenwick trees are faster than sqrt implementations,
//   which are faster than the pbds implementations

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries over a cumulative function or functor,
//   such as sum, max, and min, in 2 dimensions (sparse in 1 dimension)
// All update indices must be known beforehand
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   IndexType: the type of the index of the second dimension of the array
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutatitve
// Constructor Arguments:
//   N: the size of the first dimension of the array
//   updateInds: a vector of pairs of ints and IndexType containing the indices
//     for both dimensions for each update
//   qdef: the identity element of the operation
//   op: an instance of the Op struct
// Functions:
//   update(i, j, v): modifies the value A[i][j] to op(A[i][j], v), (i, j)
//     must be an element of updateInds
//   query(d, r): queries the cumulative value of the range [0, d] in the
//     first dimension and [0, r] in the second dimension
//   query(d, l, r, inv): queries the cumulative value of the range [0, d]
//     in the first dimension and [l, r] in the second dimension, where inv is
//     the inverse of op (minus<T>() by default)
//   query(u, d, l, r, inv): queries the cumulative value of the range [u, d]
//     in the first dimension and [l, r] in the second dimension, where inv is
//     the inverse of op (minus<T>() by default)
// In practice, has a small constant
// Time Complexity:
//   constructor: O(Q (log Q + log N) + N) for U updates
//   update, query: O(log N log U) for U updates
// Memory Complexity: O(N + U log N) for U updates
// Tested:
//   https://codeforces.com/contest/1093/problem/E
//   https://codeforces.com/contest/1523/problem/G
template <class T, class IndexType, class Op = plus<T>>
struct OfflineSemiSparseFenwickTree2D {
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  int N; vector<int> st, cnt; vector<IndexType> inds; vector<T> BIT;
  T qdef; Op op;
  int getInd(int i, IndexType j) {
    return upper_bound(inds.begin() + st[i], inds.begin() + st[i] + cnt[i], j)
        - inds.begin() - st[i];
  }
  OfflineSemiSparseFenwickTree2D(int N,
                                 vector<pair<int, IndexType>> updateInds,
                                 T qdef = T(), Op op = Op())
      : N(N), st(N + 1, 0), cnt(N + 1, 0), qdef(qdef), op(op) {
    sort(updateInds.begin(), updateInds.end(),
         [&] (const pair<int, IndexType> &a, const pair<int, IndexType> &b) {
      return a.second < b.second;
    });
    vector<IndexType> last(N + 1, IndexType());
    for (auto &&u : updateInds) for (int i = u.first + 1; i <= N; i += i & -i)
      if (cnt[i] == 0 || u.second != last[i]) { cnt[i]++; last[i] = u.second; }
    for (int i = 1; i <= N; i++) st[i] = st[i - 1] + cnt[i - 1];
    inds.resize(st[N] + cnt[N]); BIT.resize(st[N] + cnt[N], qdef);
    fill(cnt.begin(), cnt.end(), 0); for (auto &&u : updateInds)
      for (int i = u.first + 1; i <= N; i += i & -i)
        if (cnt[i] == 0 || u.second != inds[st[i] + cnt[i] - 1])
          inds[st[i] + cnt[i]++] = u.second;
  }
  void update(int i, IndexType j, T v) {
    for (i++; i <= N; i += i & -i)
      for (int s = st[i], c = cnt[i], y = getInd(i, j); y <= c; y += y & -y)
        BIT[s + y - 1] = op(BIT[s + y - 1], v);
  }
  T query(int d, IndexType r) {
    T ret = qdef; for (d++; d > 0; d -= d & -d)
      for (int s = st[d], y = getInd(d, r); y > 0; y -= y & -y)
        ret = op(ret, BIT[s + y - 1]);
    return ret;
  }
  template <class Inv = minus<T>>
  T query(int d, IndexType l, IndexType r, Inv inv = Inv()) {
    return inv(query(d, r), query(d, l - 1));
  }
  template <class Inv = minus<T>>
  T query(int u, int d, IndexType l, IndexType r, Inv inv = Inv()) {
    return inv(query(d, l, r), query(u - 1, l, r));
  }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries over a cumulative function or functor,
//   such as sum, max, and min, in 2 dimensions (sparse in 2 dimensions)
// All update indices must be known beforehand
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   IndexType1: the type of the index of the first dimension of the array
//   IndexType2: the type of the index of the second dimension of the array
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutatitve
// Constructor Arguments:
//   updateInds: a vector of pairs of IndexType1 and IndexType2 containing
//     the indices for both dimensions for each update
//   qdef: the identity element of the operation
//   op: an instance of the Op struct
// Functions:
//   update(i, j, v): modifies the value A[i][j] to op(A[i][j], v), (i, j)
//     must be an element of updateInds
//   query(d, r): queries the cumulative value of the range [0, d] in the
//     first dimension and [0, r] in the second dimension
//   query(d, l, r, inv): queries the cumulative value of the range [0, d]
//     in the first dimension and [l, r] in the second dimension, where inv is
//     the inverse of op (minus<T>() by default)
//   query(u, d, l, r, inv): queries the cumulative value of the range [u, d]
//     in the first dimension and [l, r] in the second dimension, where inv is
//     the inverse of op (minus<T>() by default)
// In practice, has a small constant
// Time Complexity:
//   constructor: O(U log U) for U updates
//   update, query: O((log U)^2) for U updates
// Memory Complexity: O(U log U) for U updates
// Tested:
//   https://judge.yosupo.jp/problem/point_add_rectangle_sum
//   https://codeforces.com/contest/1093/problem/E
//   https://codeforces.com/contest/1523/problem/G
template <class T, class IndexType1, class IndexType2, class Op = plus<T>>
struct OfflineSparseFenwickTree2D {
  static_assert(is_integral<IndexType1>::value,
                "IndexType1 must be integeral");
  static_assert(is_integral<IndexType2>::value,
                "IndexType2 must be integeral");
  int U; vector<int> st, cnt; vector<IndexType1> inds1;
  vector<IndexType2> inds2; vector<T> BIT; T qdef; Op op;
  int getInd1(IndexType1 i) {
    return upper_bound(inds1.begin(), inds1.end(), i) - inds1.begin();
  }
  int getInd2(int i, IndexType2 j) {
    return upper_bound(inds2.begin() + st[i],
                       inds2.begin() + st[i] + cnt[i], j)
        - inds2.begin() - st[i];
  }
  OfflineSparseFenwickTree2D(vector<pair<IndexType1, IndexType2>> updateInds,
                             T qdef = T(), Op op = Op())
      : inds1(updateInds.size()), qdef(qdef), op(op) {
    sort(updateInds.begin(), updateInds.end(),
         [&] (const pair<IndexType1, IndexType2> &a,
              const pair<IndexType1, IndexType2> &b) {
      return a.second < b.second;
    });
    for (int i = 0; i < int(updateInds.size()); i++)
      inds1[i] = updateInds[i].first;
    sort(inds1.begin(), inds1.end());
    inds1.erase(unique(inds1.begin(), inds1.end()), inds1.end());
    U = int(inds1.size()); st.assign(U + 1, 0); cnt.assign(U + 1, 0);
    vector<IndexType2> last(U + 1, IndexType2()); for (auto &&u : updateInds)
      for (int i = getInd1(u.first); i <= U; i += i & -i)
        if (cnt[i] == 0 || u.second != last[i]) {
          cnt[i]++; last[i] = u.second;
        }
    for (int i = 1; i <= U; i++) st[i] = st[i - 1] + cnt[i - 1];
    inds2.resize(st[U] + cnt[U]); BIT.resize(st[U] + cnt[U], qdef);
    fill(cnt.begin(), cnt.end(), 0); for (auto &&u : updateInds)
      for (int i = getInd1(u.first); i <= U; i += i & -i)
        if (cnt[i] == 0 || u.second != inds2[st[i] + cnt[i] - 1])
          inds2[st[i] + cnt[i]++] = u.second;
  }
  void update(IndexType1 i, IndexType2 j, T v) {
    for (int x = getInd1(i); x <= U; x += x & -x)
      for (int s = st[x], c = cnt[x], y = getInd2(x, j); y <= c; y += y & -y)
        BIT[s + y - 1] = op(BIT[s + y - 1], v);
  }
  T query(IndexType1 d, IndexType2 r) {
    T ret = qdef; for (int x = getInd1(d); x > 0; x -= x & -x)
      for (int s = st[x], y = getInd2(x, r); y > 0; y -= y & -y)
        ret = op(ret, BIT[s + y - 1]);
    return ret;
  }
  template <class Inv = minus<T>>
  T query(IndexType1 d, IndexType2 l, IndexType2 r, Inv inv = Inv()) {
    return inv(query(d, r), query(d, l - 1));
  }
  template <class Inv = minus<T>>
  T query(IndexType1 u, IndexType1 d, IndexType2 l, IndexType2 r,
          Inv inv = Inv()) {
    return inv(query(d, l, r), query(u - 1, l, r));
  }
};

// Sparse Fenwick Tree supporting point updates (with value 1 and -1)
//   and range sum queries in 2 dimensions
//   using SqrtBufferSimple (sparse in 1 dimension)
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   IndexType: the type of the index of the second dimension of the array
// Constructor Arguments:
//   N: the size of the first dimension of the array
//   SCALE: the value to scale sqrt by (for SqrtBufferSimple)
// Functions:
//   add(i, j): add 1 to the value A[i][j]
//   rem(i, j): subtract 1 from the value A[i][j]
//   query(d, r): queries the sum of the range [0, d] in the first dimension
//     and [0, r] in the second dimension
//   query(d, l, r): queries the sum of the range [0, d] in the first dimension
//     and [l, r] in the second dimension
//   query(u, d, l, r): queries the sum of the range [u, d] in the first
//     dimension and [l, r] in the second dimension
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N)
//   add, rem: O(log N) amortized
//   query: O(log N sqrt U) amortized for U updates
// Memory Complexity: O(N + U log N) for U updates
// Tested:
//   https://codeforces.com/contest/1093/problem/E
//   https://dmoj.ca/problem/dmopc19c7p5
template <class IndexType> struct SemiSparseFenwickTree2DSimple {
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  int N; vector<SqrtBufferSimple<IndexType>> IN, OUT;
  SemiSparseFenwickTree2DSimple(int N, double SCALE = 1)
      : N(N), IN(N + 1, SqrtBufferSimple<IndexType>(SCALE)),
        OUT(N + 1, SqrtBufferSimple<IndexType>(SCALE)) {}
  void add(int i, IndexType j) {
    for (i++; i <= N; i += i & -i) IN[i].insert(j);
  }
  void rem(int i, IndexType j) {
    for (i++; i <= N; i += i & -i) OUT[i].insert(j);
  }
  int query(int d, IndexType r) {
    int ret = 0; for (d++; d > 0; d -= d & -d)
      ret += IN[d].aboveInd(r) - OUT[d].aboveInd(r);
    return ret;
  }
  int query(int d, IndexType l, IndexType r) {
    int ret = 0; for (d++; d > 0; d -= d & -d)
      ret += IN[d].count(l, r) - OUT[d].count(l, r);
    return ret;
  }
  int query(int u, int d, IndexType l, IndexType r) {
    return query(d, l, r) - query(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range sum queries in 2 dimensions using SqrtBuffer (sparse in
//   1 dimension)
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   IndexType: the type of the index of the second dimension of the array
// Constructor Arguments:
//   N: the size of the first dimension of the array
//   SCALE: the value to scale sqrt by (for SqrtBuffer)
// Functions:
//   update(i, j, v): add v to the value A[i][j]
//   query(d, r): queries the sum of the range [0, d] in the first dimension
//     and [0, r] in the second dimension
//   query(d, l, r): queries the sum of the range [0, d] in the first dimension
//     and [l, r] in the second dimension
//   query(u, d, l, r): queries the sum of the range [u, d] in the first
//     dimension and [l, r] in the second dimension
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N)
//   update: O(log N) amortized
//   query: O(log N sqrt U) amortized for U updates
// Memory Complexity: O(N + U log N) for U updates
// Tested:
//   https://dmoj.ca/problem/apio19p3
//   https://dmoj.ca/problem/ioi01p1
template <class T, class IndexType> struct SemiSparseFenwickTree2D {
  static_assert(is_integral<IndexType>::value, "IndexType must be integeral");
  int N; vector<SqrtBuffer<T, IndexType>> BIT;
  SemiSparseFenwickTree2D(int N, double SCALE = 1)
      : N(N), BIT(N + 1, SqrtBuffer<T, IndexType>(SCALE)) {}
  void update(int i, IndexType j, T v) {
    for (i++; i <= N; i += i & -i) BIT[i].emplace(j, v);
  }
  T query(int d, IndexType r) {
    T ret = T(); for (d++; d > 0; d -= d & -d) ret += BIT[d].aboveInd(r);
    return ret;
  }
  T query(int d, IndexType l, IndexType r) {
    T ret = T(); for (d++; d > 0; d -= d & -d) ret += BIT[d].count(l, r);
    return ret;
  }
  T query(int u, int d, IndexType l, IndexType r) {
    return query(d, l, r) - query(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with value 1 and -1)
//   and range sum queries in 2 dimensions
//   using pbds hash_table for the first dimension,
//   and SqrtBufferSimple for the second dimension (sparse in 2 dimensions)
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   IndexType1: the type of the index of the first dimension of the array
//   IndexType2: the type of the index of the second dimension of the array
//   Container: the container to represent an unordered mapping from
//     IndexType1 to pair of SqrtBufferSimple<IndexType2>
// Constructor Arguments:
//   N: the size of the first dimension of the array
//   SCALE: the value to scale sqrt by (for SqrtBufferSimple)
// Functions:
//   add(i, j): add 1 to the value A[i][j]
//   rem(i, j): subtract 1 from the value A[i][j]
//   query(d, r): queries the sum of the range [0, d] in the first dimension
//     and [0, r] in the second dimension
//   query(d, l, r): queries the sum of the range [0, d] in the first dimension
//     and [l, r] in the second dimension
//   query(u, d, l, r): queries the sum of the range [u, d] in the first
//     dimension and [l, r] in the second dimension
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(1)
//   add, rem: O(log N) amortized
//   query: O(log N sqrt U) amortized for U updates
// Memory Complexity: O(U log N) for U updates
// Tested:
//   https://codeforces.com/contest/1093/problem/E
template <class IndexType1, class IndexType2,
          class Container = hashmap<
              IndexType1, pair<SqrtBufferSimple<IndexType2>,
                               SqrtBufferSimple<IndexType2>>>>
struct SparseFenwickTree2DSimple {
  static_assert(is_integral<IndexType1>::value,
                "IndexType1 must be integeral");
  static_assert(is_integral<IndexType2>::value,
                "IndexType2 must be integeral");
  IndexType1 N; double SCALE; Container BIT;
  SparseFenwickTree2DSimple(IndexType1 N, double SCALE = 1)
      : N(N), SCALE(SCALE) {}
  void add(IndexType1 i, IndexType2 j) {
    i++; for (IndexType1 x = i; x <= N; x += x & -x) {
      auto it = BIT.find(x); if (it == BIT.end()) {
        BIT[x] = make_pair(SqrtBufferSimple<IndexType2>(SCALE),
                           SqrtBufferSimple<IndexType2>(SCALE));
      }
      BIT[x].first.insert(j);
    }
  }
  void rem(IndexType1 i, IndexType2 j) {
    i++; for (IndexType1 x = i; x <= N; x += x & -x) {
      auto it = BIT.find(x); if (it == BIT.end()) {
        BIT[x] = make_pair(SqrtBufferSimple<IndexType2>(SCALE),
                           SqrtBufferSimple<IndexType2>(SCALE));
      }
      BIT[x].second.insert(j);
    }
  }
  int query(IndexType1 d, IndexType2 r) {
    d++; int ret = 0; for (IndexType1 x = d; x > 0; x -= x & -x) {
      auto it = BIT.find(x);
      ret += it->second.first.aboveInd(r) - it->second.second.aboveInd(r);
    }
    return ret;
  }
  int query(IndexType1 d, IndexType2 l, IndexType2 r) {
    d++; int ret = 0; for (IndexType1 x = d; x > 0; x -= x & -x) {
      auto it = BIT.find(x);
      ret += it->second.first.count(l, r) - it->second.second.count(l, r);
    }
    return ret;
  }
  int query(IndexType1 u, IndexType1 d, IndexType2 l, IndexType2 r) {
    return query(d, l, r) - query(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range sum queries in 2 dimensions
//   using pbds hash_table for the first dimension,
//   and SqrtBuffer for the second dimension (sparse in 2 dimensions)
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   IndexType1: the type of the index of the first dimension of the array
//   IndexType2: the type of the index of the second dimension of the array
//   Container: the container to represent an unordered mapping from
//     IndexType1 to SqrtBuffer<IndexType2, T>
// Constructor Arguments:
//   N: the size of the first dimension of the array
//   SCALE: the value to scale sqrt by (for SqrtBuffer)
// Functions:
//   update(i, j, v): add v to the value A[i][j]
//   query(d, r): queries the sum of the range [0, d] in the first dimension
//     and [0, r] in the second dimension
//   query(d, l, r): queries the sum of the range [0, d] in the first dimension
//     and [l, r] in the second dimension
//   query(u, d, l, r): queries the sum of the range [u, d] in the first
//     dimension and [l, r] in the second dimension
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(1)
//   update: O(log N) amortized
//   query: O(log N sqrt U) amortized for U updates
// Memory Complexity: O(U log N) for U update
// Tested:
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType1, class IndexType2,
          class Container = hashmap<IndexType1, SqrtBuffer<IndexType2, T>>>
struct SparseFenwickTree2D {
  static_assert(is_integral<IndexType1>::value,
                "IndexType1 must be integeral");
  static_assert(is_integral<IndexType2>::value,
                "IndexType2 must be integeral");
  IndexType1 N; double SCALE; Container BIT;
  SparseFenwickTree2D(IndexType1 N, double SCALE = 1) : N(N), SCALE(SCALE) {}
  void update(IndexType1 i, IndexType2 j, T v) {
    i++; for (IndexType1 x = i; x <= N; x += x & -x) {
      auto it = BIT.find(x); if (it == BIT.end()) {
        (BIT[x] = SqrtBuffer<IndexType2, T>(SCALE)).emplace(j, v);
      } else it->second.emplace(j, v);
    }
  }
  T query(IndexType1 d, IndexType2 r) {
    d++; T ret = T(); for (IndexType1 x = d; x > 0; x -= x & -x) {
      auto it = BIT.find(x);
      if (it != BIT.end()) ret += it->second.aboveInd(r);
    }
    return ret;
  }
  T query(IndexType1 d, IndexType2 l, IndexType2 r) {
    d++; T ret = T(); for (IndexType1 x = d; x > 0; x -= x & -x) {
      auto it = BIT.find(x);
      if (it != BIT.end()) ret += it->second.count(l, r);
    }
    return ret;
  }
  T query(IndexType1 u, IndexType1 d, IndexType2 l, IndexType2 r) {
    return query(d, l, r) - query(u - 1, l, r);
  }
};
