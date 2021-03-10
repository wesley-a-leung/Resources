#pragma once
#include <bits/stdc++.h>
#include "../lowestcommonancestor/LowestCommonAncestor.h"
using namespace std;

// Supports vertex updates and path queries on a forest
// Vertices and indices are 0-indexed
// Template Arguments:
//   R: struct supporting range updates and queries on indices
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static merge(l, r): merges the datas l and r
//       static invData(v): returns the inverse of v of type Data
//       static invLazy(v): returns the inverse of v of type Lazy
//       constructor(A): takes a vector A of type Data with the initial
//         value of each index
//       update(i, val): updates the index i with the value val
//       query(l, r): queries the range [l, r]
//     Sample Struct: supporting range sum updates and queries
//       struct R {
//         using Data = int;
//         using Lazy = int;
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         static Data invData(const Data &v) { return -v; }
//         static Lazy invLazy(const Lazy &v) { return -v; }
//         FenwickTree1D<Data> FT;
//         R(const vector<Data> &A) : FT(A.begin(), A.end()) {}
//         void update(int i, const Lazy &val) { FT.update(i, val); }
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
// Functions:
//   updateVertex(v, val): updates the vertex v with the value val
//   queryPathFromRoot(v): queries the path betwen vertex v and the root of
//     its connected component
//   queryPath(v, w): queries the path between vertices v and w
// In practice, constructor has a moderate constant,
//   update and query functions have a very small constant plus
//   the constants of R's update and query functions
// Time Complexity:
//   constructor: O(V) + time complexity of R's constructor
//   updateVertex, queryPathFromRoot, queryPath:
//     time complexity of update/query
// Memory Complexity: O(V) + memory complexity of R
// Tested:
//   https://judge.yosupo.jp/problem/vertex_add_path_sum
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=921
template <class R, const bool VALUES_ON_EDGES>
struct InvertibleVertexUpdatesPathQueries {
  using Data = typename R::Data; using Lazy = typename R::Lazy;
  LCA<> lca; int V, ind; vector<int> par, pre, post, vert; R ops;
  void updateVertex(int v, const Lazy &val) {
    ops.update(pre[v], val); if (post[v] + 1 <= post[lca.root[v]])
      ops.update(post[v] + 1, R::invLazy(val));
  }
  Data queryPathFromRoot(int v) { return ops.query(pre[lca.root[v]], pre[v]); }
  Data queryPath(int v, int w) {
    Data ret = R::merge(queryPathFromRoot(v), queryPathFromRoot(w));
    int u = lca.lca(v, w); Data inv = queryPathFromRoot(u);
    if (VALUES_ON_EDGES) inv = R::merge(inv, inv);
    else if (par[u] != -1) inv = R::merge(inv, queryPathFromRoot(par[u]));
    return R::merge(ret, R::invData(inv));
  }
  template <class Forest> void dfs(const Forest &G, int v, int prev) {
    par[v] = prev; vert[pre[v] = ++ind] = v;
    for (int w : G[v]) if (w != prev) dfs(G, w, v);
    post[v] = ind;
  }
  template <class Forest>
  vector<Data> reorder(const Forest &G, const vector<Data> &A,
                       const vector<int> &roots) {
    if (roots.empty()) {
      for (int v = 0; v < V; v++) if (par[v] == -1) dfs(G, v, -1);
    } else for (int v : roots) dfs(G, v, -1);
    vector<Data> ret; ret.reserve(V);
    for (int i = 0; i < V; i++) ret.push_back(A[vert[i]]);
    for (int v = 0; v < V; v++) if (post[v] + 1 <= post[lca.root[v]]) {
      ret[post[v] + 1] = R::merge(ret[post[v] + 1], R::invData(A[v]));
    }
    return ret;
  }
  template <class Forest> InvertibleVertexUpdatesPathQueries(
      const Forest &G, const vector<Data> &A,
      const vector<int> &roots = vector<int>())
      : lca(G, roots), V(G.size()), ind(-1), par(V, -1),
        pre(V), post(V), vert(V), ops(reorder(G, A, roots)) {}
  template <class Forest> InvertibleVertexUpdatesPathQueries(
      const Forest &G, const vector<Data> &A, int rt)
      : InvertibleVertexUpdatesPathQueries(G, A, vector<int>{rt}) {}
};
