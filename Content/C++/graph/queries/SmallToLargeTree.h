#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries on graphs that involve the set of vertices in a subtree
// Template Arguments:
//   S: struct to maintain a multiset of the elements in a set
//   Required Fields:
//     T: the type of each element
//     R: the type of the return value for each query
//     Q: the query object that contains information for each query
//       Required Fields:
//         v: the vertex of the subtree to query
//   Required Functions:
//     constructor(A): takes a vector A of type T equal to the static array
//       representing the values for each vertex of the graph
//     add(v): adds the value v to the multiset
//     remove(v): removes the value v from the multiset
//   Sample Struct: supporting queries for whether a value c exists in
//       the subtree of vertex v
//     struct S {
//       using T = int; using R = bool;
//       struct Q { int v, c; };
//       vector<T> cnt;
//       S(const vector<T> &A)
//           : cnt(*max_element(A.begin(), A.end()) + 1, 0) {}
//       void add(const T &v) { cnt[v]++; }
//       void remove(const T &v) { --cnt[v]; }
//       R query(const Q &q) const {
//         return 0 <= q.c && q.c < int(cnt.size()) && cnt[q.c] > 0;
//       }
//     };
// Constructor Arguments:
//   G: a generic forest data structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
//   A: a vector of type S::T of the values in the array
//   queries: a vector of type S::Q representing the queries
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
//   f: a function from S::T to any arithmetic type, f(A[v]) needs to return
//     the weight of the vertex v with value A[v]
// Fields:
//   ans: a vector of integers with the answer for each query
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(C + U V log V + K T)
//     for K queries where C is the time complexity of S's constructor,
//     U is the time complexity of S.add and S.remove,
//     and T is the time complexity of S.query
// Memory Complexity: O(V + K) for K queries
// Tested:
//   https://cses.fi/problemset/task/1139/
//   https://codeforces.com/contest/600/problem/E
//   https://codeforces.com/contest/375/problem/D
template <class S> struct SmallToLargeTree {
  using T = typename S::T; using R = typename S::R; using Q = typename S::Q;
  vector<R> ans;
  template <class Forest, class F = function<int(T)>> SmallToLargeTree(
      const Forest &G, const vector<T> &A, const vector<Q> &queries,
      const vector<int> &roots = vector<int>(), F f = [] (T) { return 1; } ) {
    using weight_t = decltype(f(T())); int V = G.size(), K = queries.size();
    vector<int> st(V + 1, 0), ind(K); S s(A);
    vector<weight_t> size(V, weight_t());
    function<void(int, int)> getSize = [&] (int v, int prev) {
      size[v] = f(A[v]);
      for (int w : G[v]) if (w != prev) { getSize(w, v); size[v] += size[w]; }
    };
    function<void(int, int, int, bool)> update = [&] (
        int v, int prev, int heavy, bool add) {
      if (add) s.add(A[v]);
      else s.remove(A[v]);
      for (int w : G[v]) if (w != prev && w != heavy) update(w, v, heavy, add);
    };
    function<void(int, int, bool)> dfs = [&] (int v, int prev, bool keep) {
      int heavy = -1; for (int w : G[v]) 
        if (w != prev && (heavy == -1 || size[heavy] < size[w])) heavy = w;
      for (int w : G[v]) if (w != prev && w != heavy) dfs(w, v, 0);
      if (heavy != -1) dfs(heavy, v, 1);
      update(v, prev, heavy, 1); for (int i = st[v]; i < st[v + 1]; i++) {
        R res = s.query(queries[ind[i]]); if (ans.empty()) ans.resize(K, res);
        ans[ind[i]] = res;
      }
      if (!keep) update(v, prev, -1, 0);
    };
    for (int i = 0; i < K; i++) st[queries[i].v]++;
    partial_sum(st.begin(), st.end(), st.begin());
    for (int i = 0; i < K; i++) ind[--st[queries[i].v]] = i;
    if (roots.empty()) {
      for (int v = 0; v < V; v++)
        if (size[v] == weight_t()) { getSize(v, -1); dfs(v, -1, 0); }
    } else for (int v : roots) { getSize(v, -1); dfs(v, -1, 0); }
  }
  template <class Forest, class F = function<int(T)>> SmallToLargeTree(
      const Forest &G, const vector<T> &A, const vector<Q> &queries, int rt,
      F f = [] (T) { return 1; })
      : SmallToLargeTree(G, A, queries, vector<int>{rt}, f) {}
};
