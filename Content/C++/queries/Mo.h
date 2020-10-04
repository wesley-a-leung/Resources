#pragma once
#include <bits/stdc++.h>
using namespace std;

// Mo's algorithm to answer offline ranges queries over
//   a static array A of length N
// Template Arguments:
//   S: struct to maintain a multiset of the elements in the subarray [l, r]
//   Required Fields:
//     T: typedef/using for the type of each element
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
//     query(): returns the answer for a query to the multiset
//   Sample Struct: supporting queries for the number of inversions
//       in an integer array with a small maximum value
//     struct S {
//       using T = int;
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
//       long long query() { return inversions; }
//     };
// Constructor Arguments:
//   A: a vector of type S::T of the values in the array
//   queries: a vector of pairs containing the inclusive endpoints of
//     the queries
// Fields:
//   ans: a vector of integers with the answer for each query
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(C + Q (U (log Q + sqrt N) + T))
//     where C is the time complexity of S's constructor,
//     U is the time complexity of S.add and S.remove,
//     and T is the time compexity of S.query
// Memory Complexity: O(Q)
// Tested:
//   https://www.spoj.com/problems/DQUERY/
//   https://atcoder.jp/contests/abc174/tasks/abc174_f
//   https://judge.yosupo.jp/problem/static_range_inversions_query
template <class S> struct Mo {
  struct Query {
    int l, r, i, b;
    Query(int l, int r, int i, int b) : l(l), r(r), i(i), b(b) {}
    bool operator < (const Query &q) const {
      return b == q.b ? r < q.r : b < q.b;
    }
  };
  vector<decltype(S(vector<typename S::T>()).query())> ans;
  Mo(const vector<typename S::T> &A, const vector<pair<int, int>> &queries,
     double SCALE = 2) {
    int Q = queries.size(), bsz = sqrt(A.size()) * SCALE;
    vector<Query> q; q.reserve(Q); for (int i = 0; i < Q; i++) {
      int l, r; tie(l, r) = queries[i]; q.emplace_back(l, r, i, l / bsz);
    }
    sort(q.begin(), q.end()); S s(A); ans.assign(Q, s.query());
    int l = 0, r = l - 1; for (auto &&qi : q) {
      while (l < qi.l) s.removeLeft(A[l++]);
      while (l > qi.l) s.addLeft(A[--l]);
      while (r < qi.r) s.addRight(A[++r]);
      while (r > qi.r) s.removeRight(A[r--]);
      ans[qi.i] = s.query();
    }
  }
};
