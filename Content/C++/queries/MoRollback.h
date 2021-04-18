#pragma once
#include <bits/stdc++.h>
using namespace std;

// Mo's algorithm to answer offline ranges queries over
//   a static array A of length N where elements can be only added to a
//   multiset-like object
// Template Arguments:
//   S: struct to maintain a multiset of elements
//   Required Fields:
//     T: the type of each element
//     R: the type of the return value for each query
//     Q: the query object that contains information for each query
//       Required Fields:
//         l: the left endpoint of the query range
//         r: the right endpoint of the query range
//   Required Functions:
//     constructor(...args): takes any number of arguments (arguments are
//       passed from constructor of Mo)
//     add(v): adds the value v to the multiset
//     save(): saves the current state of the multiset
//     rollback(): rollbacks the multiset to the saved state
//     reset(): resets the multiset to its initial state
//     query(q): returns the answer of type R for the query q of type Q to
//       the multiset, which is guaranteed to contain all elements in the range
//       [q.l, q.r] and only those elements
//   Sample Struct: supporting queries for 0-1 knapsack problem with a size of
//       q.w over a subarray of items with a weight and value
//     struct S {
//       using T = pair<int, int>; using R = int;
//       struct Q { int l, r, w; };
//       int MAXW; vector<int> dp, saved;
//       S(int MAXW) : MAXW(MAXW), dp(MAXW + 1) {}
//       void add(const T &v) {
//         for (int j = MAXW; j >= v.first; j--)
//           dp[j] = max(dp[j], dp[j - v.first] + v.second);
//       }
//       void save() { saved = dp; }
//       void rollback() { dp = saved; }
//       void reset() { fill(dp.begin(), dp.end(), 0); }
//       R query(const Q &q) const { return dp[q.w]; }
//     };
// Constructor Arguments:
//   A: a vector of type S::T of the values in the array
//   queries: a vector of type S::Q representing the queries
//   SCALE: the value to scale sqrt by
//   ...args: arguments to pass to the constructor of S
// Fields:
//   ans: a vector of integers with the answer for each query
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(C + K ((log K + A sqrt N) + R + T) + D sqrt N)
//     for K queries where C is the time complexity of S's constructor,
//     A is the time complexity of S.add, T is the time complexity of S.query,
//     R is the time complexity of S.rollback, and D is the time complexity of
//     S.reset
// Memory Complexity: O(K + M) for K queries where M is the memory complexity
//   of S
// Tested:
//   https://www.spoj.com/problems/DQUERY/
//   https://dmoj.ca/problem/vpex1p5
template <class S> struct MoRollback {
  using T = typename S::T; using R = typename S::R; using Q = typename S::Q;
  struct Query {
    Q q; int i, b; Query(const Q &q, int i, int b) : q(q), i(i), b(b) {}
    pair<int, int> getPair() const { return make_pair(b, q.r); }
    bool operator < (const Query &o) const { return getPair() < o.getPair(); }
  };
  vector<R> ans;
  template <class ...Args> MoRollback(
      const vector<T> &A, const vector<Q> &queries, double SCALE = 1,
      Args &&...args) {
    int N = A.size(), K = queries.size(), bsz = max(1.0, sqrt(N) * SCALE);
    vector<Query> q; q.reserve(K); S s(forward<Args>(args)...);
    int l = 0, r = -1, last = -1; for (int i = 0; i < K; i++)
      q.emplace_back(queries[i], i, queries[i].l / bsz);
    sort(q.begin(), q.end()); for (auto &&qi : q) {
      if (qi.b != last) {
        l = min(N, ((last = qi.b) + 1) * bsz); r = l - 1; s.reset(); s.save();
      }
      if (qi.q.r < l) { for (int i = qi.q.l; i <= qi.q.r; i++) s.add(A[i]); }
      else {
        while (r < qi.q.r) s.add(A[++r]);
        s.save(); for (int i = l - 1; i >= qi.q.l; i--) s.add(A[i]);
      }
      R res = s.query(qi.q); if (ans.empty()) ans.resize(K, res);
      ans[qi.i] = res; s.rollback();
    }
  }
};
