#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports subtree updates and queries on a forest
// Vertices and indices are 0-indexed
// Template Arguments:
//   R: struct supporting range updates and queries on indices
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       constructor(A): takes a vector A of type Data with the initial
//         value of each index
//       update(l, r, val): updates the range [l, r] with the value val
//       query(l, r): queries the range [l, r]
//     Sample Struct: supporting range sum updates and queries
//       struct R {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         FenwickTreeRange1D<Data> FT;
//         R(vector<Data> A) : FT(move(A)) {}
//         void update(int l, int r, const Lazy &val) { FT.update(l, r, val); }
//         Data query(int l, int r) { return FT.query(l, r); }
//       };
//   VALUES_ON_EDGES: boolean indicating whether the values are on the edges
//     (the largest depth vertex of the edge) or the vertices
// Constructor Arguments:
//   G: a generic forest data structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
//   A: a vector of type R::Data with the initial value of each vertex
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
// Fields:
//   pre: vector of the pre order traversal indices for each vertex
//   post: vector of the post order traversal indices (the last pre order index
//     in its subtree) for each vertex
//   vert: vector of vertex for each pre order index
// Functions:
//   updateVertex(v, val): updates the vertex v with the value val
//   updateSubtree(v, val): updates the subtree of vertex v with the value val
//   queryVertex(v): queries the vertex v
//   querySubtree(v): queries the subtree of vertex v
// In practice, constructor has a moderate constant,
//   update and query functions have a very small constant plus
//   the constants of R's update and query functions
// Time Complexity:
//   constructor: O(V) + time complexity of R's constructor
//   updateVertex, updateSubtree, queryVertex, querySubtree:
//     time complexity of update/query
// Memory Complexity: O(V) + memory complexity of R
// Tested:
//   https://judge.yosupo.jp/problem/vertex_add_subtree_sum
//   https://codeforces.com/contest/620/problem/E
template <class R, const bool VALUES_ON_EDGES> struct SubtreeQueries {
  using Data = typename R::Data; using Lazy = typename R::Lazy;
  int V, ind; vector<int> pre, post, vert; R ops;
  void updateVertex(int v, const Lazy &val) {
    ops.update(pre[v], pre[v], val);
  }
  void updateSubtree(int v, const Lazy &val) {
    int l = pre[v] + VALUES_ON_EDGES, r = post[v];
    if (l <= r) ops.update(l, r, val);
  }
  Data queryVertex(int v) { return ops.query(pre[v], pre[v]); }
  Data querySubtree(int v) {
    int l = pre[v] + VALUES_ON_EDGES, r = post[v];
    return l <= r ? ops.query(l, r) : R::qdef();
  }
  template <class Forest> void dfs(const Forest &G, int v, int prev) {
    vert[pre[v] = ++ind] = v; for (int w : G[v]) if (w != prev) dfs(G, w, v);
    post[v] = ind;
  }
  template <class Forest>
  vector<Data> reorder(const Forest &G, const vector<Data> &A,
                       const vector<int> &roots) {
    if (roots.empty()) {
      for (int v = 0; v < V; v++) if (pre[v] == -1) dfs(G, v, -1);
    } else for (int v : roots) dfs(G, v, -1);
    vector<Data> ret; ret.reserve(A.capacity());
    for (int i = 0; i < V; i++) ret.push_back(A[vert[i]]);
    return ret;
  }
  template <class Forest>
  SubtreeQueries(const Forest &G, const vector<Data> &A,
                 const vector<int> &roots = vector<int>())
      : V(G.size()), ind(-1), pre(V, -1), post(V), vert(V),
        ops(reorder(G, A, roots)) {}
  template <class Forest>
  SubtreeQueries(const Forest &G, const vector<Data> &A, int rt)
      : SubtreeQueries(G, A, vector<int>{rt}) {}
};
