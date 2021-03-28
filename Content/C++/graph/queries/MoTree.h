#pragma once
#include <bits/stdc++.h>
#include "LowestCommonAncestor.h"
using namespace std;

// Mo's algorithm on a tree to answer offline queries over paths on a forest
//   where each vertex has a value provided by an array A
// Template Arguments:
//   S: struct to maintain a multiset of the elements in a set
//   Required Fields:
//     T: the type of each element
//     R: the type of the return value for each query
//     Q: the query object that contains information for each query
//       Required Fields:
//         v: one vertex of the query path
//         w: the other vertex of the query path
//   Required Functions:
//     constructor(A): takes a vector A of type T equal to the static array
//       representing the values for each vertex of the graph
//     add(v): adds the value v to the multiset
//     remove(v): removes the value v from the multiset
//   Sample Struct: supporting queries for whether a value c exists on
//       a path between vertices v and w
//     struct S {
//       using T = int; using R = bool;
//       struct Q { int v, w, c; };
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
// Fields:
//   ans: a vector of integers with the answer for each query
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(C + K (U (log K + sqrt V) + T))
//     for K queries where C is the time complexity of S's constructor,
//     U is the time complexity of S.add and S.remove,
//     and T is the time complexity of S.query
// Memory Complexity: O(V + K) for K queries
// Tested:
//   https://www.spoj.com/problems/COT2/
//   https://www.spoj.com/problems/GOT/
//   https://dmoj.ca/problem/year2018p6
template <class S> struct MoTree {
  using T = typename S::T; using R = typename S::R; using Q = typename S::Q;
  struct Query {
    Q q; int l, r, lca, i, b;
    Query(const Q &q, int l, int r, int lca, int i, int b)
        : q(q), l(l), r(r), lca(lca), i(i), b(b) {}
    pair<int, int> getPair() const { return make_pair(b, b % 2 ? -r : r); }
    bool operator < (const Query &o) const { return getPair() < o.getPair(); }
  };
  int V, ind; vector<int> pre, post, vert; LCA<> lca; vector<R> ans;
  template <class Forest> void dfs(const Forest &G, int v, int prev) {
    vert[pre[v] = ind++] = v; for (int w : G[v]) if (w != prev) dfs(G, w, v);
    vert[post[v] = ind++] = v;
  }
  template <class Forest> LCA<> init(const Forest &G) {
    vector<int> roots; for (int v = 0; v < V; v++)
      if (pre[v] == -1) { roots.push_back(v); dfs(G, v, -1); }
    return LCA<>(G, roots);
  }
  template <class Forest> MoTree(const Forest &G, const vector<T> &A,
                                 const vector<Q> &queries, double SCALE = 2)
      : V(G.size()), ind(0), pre(V, -1), post(V), vert(V * 2), lca(init(G)) {
    int K = queries.size(), bsz = max(1.0, sqrt(A.size()) * SCALE);
    vector<Query> q; q.reserve(K); vector<bool> vis(V, false); S s(A);
    for (int i = 0; i < K; i++) {
      int v = queries[i].v, w = queries[i].w, u = lca.lca(v, w);
      if (pre[v] > pre[w]) swap(v, w);
      int l = u == v ? pre[v] : post[v], r = pre[w];
      q.emplace_back(queries[i], l, r, u, i, l / bsz);
    }
    auto update = [&] (int v) {
      if (vis[v]) s.remove(A[v]);
      else s.add(A[v]);
      vis[v] = !vis[v];
    };
    sort(q.begin(), q.end()); int l = 0, r = l - 1; for (auto &&qi : q) {
      while (l < qi.l) update(vert[l++]);
      while (l > qi.l) update(vert[--l]);
      while (r < qi.r) update(vert[++r]);
      while (r > qi.r) update(vert[r--]);
      if (qi.lca != vert[l] && qi.lca != vert[r]) update(qi.lca);
      R res = s.query(qi.q); if (ans.empty()) ans.resize(K, res);
      ans[qi.i] = res;
      if (qi.lca != vert[l] && qi.lca != vert[r]) update(qi.lca);
    }
  }
};
