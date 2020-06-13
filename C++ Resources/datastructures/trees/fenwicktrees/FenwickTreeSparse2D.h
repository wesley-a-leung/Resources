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
// Offline fenwick trees and sqrt implementations have small constants,
//   pbds tree implementation has a moderate constant,
//   and pbds hash_table implementations have large constants

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries in 2 dimensions (sparse in 1 dimension)
// All update indices must be known beforehand
// Indices are 0-indexed and ranges are inclusive
// In practice, has a small constant
// Time Complexity:
//   constructor: O(Q (log Q + log N) + N) for Q updates
//   update, rsq: O(log N log Q) for Q updates
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://dmoj.ca/problem/dmopc19c7p5
//   https://codeforces.com/contest/1093/problem/E
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType> struct FenwickTreeSemiSparse2DOffline {
  int N; vector<int> st, cnt; vector<IndexType> inds; vector<T> BIT;
  int getInd(int i, IndexType j) {
    return upper_bound(inds.begin() + st[i], inds.begin() + st[i] + cnt[i], j)
        - inds.begin() - st[i];
  }
  FenwickTreeSemiSparse2DOffline(int N,
                                 vector<pair<int, IndexType>> updateInds)
      : N(N), st(N + 1, 0), cnt(N + 1, 0) {
    sort(updateInds.begin(), updateInds.end(),
         [&] (const pair<int, IndexType> &a, const pair<int, IndexType> &b) {
      return a.second < b.second;
    });
    vector<IndexType> last(N + 1, T());
    for (auto &&u : updateInds) for (int i = u.first + 1; i <= N; i += i & -i)
      if (cnt[i] == 0 || u.second != last[i]) { cnt[i]++; last[i] = u.second; }
    for (int i = 1; i <= N; i++) st[i] = st[i - 1] + cnt[i - 1];
    inds.resize(st[N] + cnt[N]); BIT.resize(st[N] + cnt[N]);
    fill(cnt.begin(), cnt.end(), 0); for (auto &&u : updateInds)
      for (int i = u.first + 1; i <= N; i += i & -i)
        if (cnt[i] == 0 || u.second != inds[st[i] + cnt[i] - 1])
          inds[st[i] + cnt[i]++] = u.second;
  }
  void update(int i, IndexType j, T v) {
    for (i++; i <= N; i += i & -i)
      for (int s = st[i], c = cnt[i], y = getInd(i, j); y <= c; y += y & -y)
        BIT[s + y - 1] += v;
  }
  T rsq(int d, IndexType r) {
    T ret = T(); for (d++; d > 0; d -= d & -d)
      for (int s = st[d], y = getInd(d, r); y > 0; y -= y & -y)
        ret += BIT[s + y - 1];
    return ret;
  }
  T rsq(int d, IndexType l, IndexType r) { return rsq(d, r) - rsq(d, l - 1); }
  T rsq(int u, int d, IndexType l, IndexType r) {
    return rsq(d, l, r) - rsq(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries in 2 dimensions (sparse in 2 dimensions)
// All update indices must be known beforehand
// Indices are 0-indexed and ranges are inclusive
// In practice, has a small constant
// Time Complexity:
//   constructor: O(Q log Q) for Q updates
//   update, rsq: O((log Q)^2) for Q updates
// Memory Complexity: O(Q log Q) for Q updates
// Tested:
//   https://judge.yosupo.jp/problem/point_add_rectangle_sum
//   https://dmoj.ca/problem/dmopc19c7p5
//   https://codeforces.com/contest/1093/problem/E
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType1, class IndexType2>
struct FenwickTreeSparse2DOffline {
  int U; vector<int> st, cnt; vector<IndexType1> inds1;
  vector<IndexType2> inds2; vector<T> BIT;
  int getInd1(IndexType1 i) {
    return upper_bound(inds1.begin(), inds1.end(), i) - inds1.begin();
  }
  int getInd2(int i, IndexType2 j) {
    return upper_bound(inds2.begin() + st[i],
                       inds2.begin() + st[i] + cnt[i], j)
        - inds2.begin() - st[i];
  }
  FenwickTreeSparse2DOffline(vector<pair<IndexType1, IndexType2>> updateInds)
      : inds1(updateInds.size()) {
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
    vector<IndexType2> last(U + 1, T()); for (auto &&u : updateInds)
      for (int i = getInd1(u.first); i <= U; i += i & -i)
        if (cnt[i] == 0 || u.second != last[i]) {
          cnt[i]++; last[i] = u.second;
        }
    for (int i = 1; i <= U; i++) st[i] = st[i - 1] + cnt[i - 1];
    inds2.resize(st[U] + cnt[U]); BIT.resize(st[U] + cnt[U]);
    fill(cnt.begin(), cnt.end(), 0); for (auto &&u : updateInds)
      for (int i = getInd1(u.first); i <= U; i += i & -i)
        if (cnt[i] == 0 || u.second != inds2[st[i] + cnt[i] - 1])
          inds2[st[i] + cnt[i]++] = u.second;
  }
  void update(IndexType1 i, IndexType2 j, T v) {
    for (int x = getInd1(i); x <= U; x += x & -x)
      for (int s = st[x], c = cnt[x], y = getInd2(x, j); y <= c; y += y & -y)
        BIT[s + y - 1] += v;
  }
  T rsq(IndexType1 d, IndexType2 r) {
    T ret = T(); for (int x = getInd1(d); x > 0; x -= x & -x)
      for (int s = st[x], y = getInd2(x, r); y > 0; y -= y & -y)
        ret += BIT[s + y - 1];
    return ret;
  }
  T rsq(IndexType1 d, IndexType2 l, IndexType2 r) {
    return rsq(d, r) - rsq(d, l - 1);
  }
  T rsq(IndexType1 u, IndexType1 d, IndexType2 l, IndexType2 r) {
    return rsq(d, l, r) - rsq(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with value 1 and -1)
//   and range queries in 2 dimensions
//   using SqrtBufferSimple (sparse in 1 dimension)
// Indices are 0-indexed and ranges are inclusive
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   add, rem: O(log N) amortized
//   rsq: O(log N sqrt Q) amortized for Q updates
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://codeforces.com/contest/1093/problem/E
//   https://dmoj.ca/problem/dmopc19c7p5
template <class IndexType> struct FenwickTreeSemiSparse2DSimpleSqrt {
  int N; vector<SqrtBufferSimple<IndexType>> IN, OUT;
  FenwickTreeSemiSparse2DSimpleSqrt(int N, double SCALE = 1)
      : N(N), IN(N + 1, SqrtBufferSimple<IndexType>(SCALE)),
        OUT(N + 1, SqrtBufferSimple<IndexType>(SCALE)) {}
  void add(int i, IndexType j) {
    for (i++; i <= N; i += i & -i) IN[i].insert(j);
  }
  void rem(int i, IndexType j) {
    for (i++; i <= N; i += i & -i) OUT[i].insert(j);
  }
  int rsq(int d, IndexType r) {
    int ret = 0; for (d++; d > 0; d -= d & -d)
      ret += IN[d].aboveInd(r) - OUT[r].aboveInd(r);
    return ret;
  }
  int rsq(int d, IndexType l, IndexType r) {
    int ret = 0; for (d++; d > 0; d -= d & -d)
      ret += IN[d].count(l, r) - OUT[d].count(l, r);
    return ret;
  }
  int rsq(int u, int d, IndexType l, IndexType r) {
    return rsq(d, l, r) - rsq(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries in 2 dimensions using SqrtBuffer (sparse in 1 dimension)
// Indices are 0-indexed and ranges are inclusive
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   update: O(log N) amortized
//   rsq: O(log N sqrt Q) amortized for Q updates
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://dmoj.ca/problem/apio19p3
//   https://dmoj.ca/problem/ioi01p1
template <class T, class IndexType> struct FenwickTreeSemiSparse2DSqrt {
  int N; vector<SqrtBuffer<T, IndexType>> BIT;
  FenwickTreeSemiSparse2DSqrt(int N, double SCALE = 1)
      : N(N), BIT(N + 1, SqrtBuffer<T, IndexType>(SCALE)) {}
  void update(int i, IndexType j, T v) {
    for (i++; i <= N; i += i & -i) BIT[i].emplace(j, v);
  }
  T rsq(int d, IndexType r) {
    T ret = T(); for (d++; d > 0; d -= d & -d) ret += BIT[d].aboveInd(r);
    return ret;
  }
  T rsq(int d, IndexType l, IndexType r) {
    T ret = T(); for (d++; d > 0; d -= d & -d) ret += BIT[d].count(l, r);
    return ret;
  }
  T rsq(int u, int d, IndexType l, IndexType r) {
    return rsq(d, l, r) - rsq(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with value 1 and -1) 
//   and range queries in 2 dimensions using pbds tree (sparse in 1 dimension)
// Indices are 0-indexed and ranges are inclusive
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N)
//   add, rem, rsq: O(log N log Q) for Q updates
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://codeforces.com/contest/1093/problem/E
template <class IndexType> struct FenwickTreeSemiSparse2DSimpleTreeset {
  int N, stamp; vector<treeset<pair<IndexType, int>>> BIT;
  FenwickTreeSemiSparse2DSimpleTreeset(int N)
      : N(N), stamp(0), BIT(N + 1, treeset<pair<IndexType, int>>()) {}
  void add(int i, IndexType j) {
    for (i++; i <= N; i += i & -i) BIT[i].insert(make_pair(j, stamp++));
  }
  void rem(int i, IndexType j) {
    for (i++; i <= N; i += i & -i)
      BIT[i].erase(BIT[i].lower_bound(make_pair(j, 0)));
  }
  int rsq(int d, IndexType r) {
    int ret = 0; for (d++; d > 0; d -= d & -d)
      ret += BIT[d].order_of_key(make_pair(r, stamp));
    return ret;
  }
  int rsq(int d, IndexType l, IndexType r) {
    return rsq(d, r) - rsq(d, l - 1);
  }
  int rsq(int u, int d, IndexType l, IndexType r) {
    return rsq(d, l, r) - rsq(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries in 2 dimensions
//   using pbds hash_table (sparse in 1 dimension)
// Indices are 0-indexed and ranges are inclusive
// In practice, has a large constant
// Time Complexity:
//   constructor: O(N)
//   update, rsq: O(log N log M) on avareage
// Memory Complexity: O(N + Q log N log M) for Q updates
// Tested:
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType, class Container = hashmap<IndexType, T>>
struct FenwickTreeSemiSparse2D {
  int N; vector<FenwickTreeSparse1D<T, IndexType, Container>> BIT;
  FenwickTreeSemiSparse2D(int N, IndexType M)
      : N(N), BIT(N + 1, FenwickTreeSparse1D<T, IndexType, Container>(M)) {}
  void update(int i, IndexType j, T v) {
    for (i++; i <= N; i += i & -i) BIT[i].update(j, v);
  }
  T rsq(int d, IndexType r) {
    T ret = T(); for (d++; d > 0; d -= d & -d) ret += BIT[d].rsq(r);
    return ret;
  }
  T rsq(int d, IndexType l, IndexType r) { return rsq(d, r) - rsq(d, l - 1); }
  T rsq(int u, int d, IndexType l, IndexType r) {
    return rsq(d, l, r) - rsq(u - 1, l, r);
  }
};

// Sparse Fenwick Tree supporting point updates (with any value)
//   and range queries in 2 dimensions
//   using pbds hash_table (sparse in 2 dimensions)
// Indices are 0-indexed and ranges are inclusive
// In practice, has a large constant
// Time Complexity:
//   constructor: O(1)
//   update, rsq: O(log N log M) on average
// Memory Complexity: O(Q log N log M) for Q updates
// Tested:
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, class IndexType1, class IndexType2,
          class Container = hashmap<pair<IndexType1, IndexType2>, T,
                                    pair_hash<IndexType1, IndexType2>>>
struct FenwickTreeSparse2D {
  IndexType1 N; IndexType2 M; Container BIT;
  FenwickTreeSparse2D(IndexType1 N, IndexType2 M) : N(N), M(M) {}
  void update(IndexType1 i, IndexType2 j, T v) {
    i++; j++; for (IndexType1 x = i; x <= N; x += x & -x) {
      for (IndexType2 y = j; y <= M; y += y & -y) {
        auto it = BIT.find(make_pair(x, y));
        if (it == BIT.end()) BIT[make_pair(x, y)] += v;
        else if ((it->second += v) == T()) BIT.erase(it->first);
      }
    }
  }
  T rsq(IndexType1 d, IndexType2 r) {
    d++; r++; T ret = T(); for (IndexType1 x = d; x > 0; x -= x & -x) {
      for (IndexType2 y = r; y > 0; y -= y & -y) {
        auto it = BIT.find(make_pair(x, y));
        if (it != BIT.end()) ret += it->second;
      }
    }
    return ret;
  }
  T rsq(IndexType1 d, IndexType2 l, IndexType2 r) {
    return rsq(d, r) - rsq(d, l - 1);
  }
  T rsq(IndexType1 u, IndexType1 d, IndexType2 l, IndexType2 r) {
    return rsq(d, l, r) - rsq(u - 1, l, r);
  }
};
