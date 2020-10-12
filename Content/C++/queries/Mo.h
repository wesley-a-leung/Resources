#pragma once
#include <bits/stdc++.h>
using namespace std;

// Mo's algorithm to answer offline ranges queries over
//   a static array A of length N
// Template Arguments:
//   S: struct to maintain a multiset of the elements in the subarray [l, r]
//   Required Fields:
//     T: the type of each element
//     R: the type of the return value for each query
//     Q: the query object that contains information for each query
//       Required Fields:
//         l: the left endpoint of the query range
//         r: the right endpoint of the query range
//   Required Functions:
//     constructor(A): takes a vector A of type T equal to the static array
//     addLeft(v): adds the value v to the multiset by expanding the subarray
//       [l, r] to [l - 1, r]
//     addRight(v): adds the value v to the multiset by expanding the subarray
//       [l, r] to [l, r + 1]
//     removeLeft(v): removes the value v from the multiset assuming
//       it exists by contracting the subarray [l, r] to [l + 1, r]
//     removeRight(v): removes the value v from the multiset assuming
//       it exists by contracting the subarray [l, r] to [l, r - 1]
//     query(q): returns the answer of type R for the query q of type Q to
//       the multiset, which is guaranteed to contain all elements in the range
//       [q.l, q.r] and only those elements
//   Sample Struct: supporting queries for the number of inversions
//       in an integer array with a small maximum value
//     struct S {
//       using T = int; using R = long long;
//       struct Q { int l, r; };
//       int U; FenwickTree1D<int> ft; long long inversions;
//       S(const vector<T> &A)
//           : U(*max_element(A.begin(), A.end()) + 1), ft(U),
//             inversions(0) {}
//       void addLeft(const T &v) {
//         inversions += ft.query(0, v - 1); ft.update(v, 1);
//       }
//       void addRight(const T &v) {
//         inversions += ft.query(v + 1, U - 1); ft.update(v, 1);
//       }
//       void removeLeft(const T &v) {
//         inversions -= ft.query(0, v - 1); ft.update(v, -1);
//       }
//       void removeRight(const T &v) {
//         inversions -= ft.query(v + 1, U - 1); ft.update(v, -1);
//       }
//       R query(const Q &q) const { return inversions; }
//     };
// Constructor Arguments:
//   A: a vector of type S::T of the values in the array
//   queries: a vector of pairs containing the inclusive endpoints of
//     the queries
// Fields:
//   ans: a vector of integers with the answer for each query
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(C + K (U (log K + sqrt N) + T))
//     for K queries where C is the time complexity of S's constructor,
//     U is the time complexity of S.addLeft, S.addRight,
//     S.removeLeft, S.removeRight,
//     and T is the time compexity of S.query
// Memory Complexity: O(K) for K queries
// Tested:
//   https://www.spoj.com/problems/DQUERY/
//   https://atcoder.jp/contests/abc174/tasks/abc174_f
//   https://judge.yosupo.jp/problem/static_range_inversions_query
template <class S> struct Mo {
  using T = typename S::T; using R = typename S::R; using Q = typename S::Q;
  struct Query {
    Q q; int i, b; Query(const Q &q, int i, int b) : q(q), i(i), b(b) {}
    bool operator < (const Query &o) const {
      return b == o.b ? q.r < o.q.r : b < o.b;
    }
  };
  vector<R> ans;
  Mo(const vector<T> &A, const vector<Q> &queries, double SCALE = 2) {
    int K = queries.size(), bsz = max(1.0, sqrt(A.size()) * SCALE);
    vector<Query> q; q.reserve(K); S s(A); for (int i = 0; i < K; i++)
      q.emplace_back(queries[i], i, queries[i].l / bsz);
    sort(q.begin(), q.end()); int l = 0, r = l - 1; for (auto &&qi : q) {
      while (l < qi.q.l) s.removeLeft(A[l++]);
      while (l > qi.q.l) s.addLeft(A[--l]);
      while (r < qi.q.r) s.addRight(A[++r]);
      while (r > qi.q.r) s.removeRight(A[r--]);
      R res = s.query(qi.q); if (ans.empty()) ans.resize(K, res);
      ans[qi.i] = res;
    }
  }
};
