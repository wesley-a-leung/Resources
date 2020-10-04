#pragma once
#include <bits/stdc++.h>
#include "../lowestcommonancestor/LowestCommonAncestor.h"
using namespace std;

// Supports path updates and vertex queries on a forest
// Vertices and indices are 0-indexed
// Template Arguments:
//   R: struct supporting range updates and queries on indices
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static merge(l, r): merges the datas l and r
//       static mergeLazy(l, r): merges the lazy values l and r
//       static invData(v): returns the inverse of v of type Data
//       static invLazy(v): returns the inverse of v of type Lazy
//       constructor(A): takes a vector A of type Data with the initial
//         value of each index
//       update(l, r, val): updates the range [l, r] with the value val
//       query(i): queries the index i
//     Sample Struct: supporting range sum updates and queries
//       struct R {
//         using Data = int;
//         using Lazy = int;
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         static Lazy mergeLazy(const Lazy &l, const Lazy &r) {
//           return l + r;
//         }
//         static Data invData(const Data &v) { return -v; }
//         static Lazy invLazy(const Lazy &v) { return -v; }
//         FenwickTreeRangePoint1D<Data> FT;
//         R(const vector<Data> &A) : FT(A.begin(), A.end()) {}
//         void update(int l, int r, const Lazy &val) { FT.update(l, r, val); }
//         Data query(int i) { return FT.get(i); }
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
// Functions:
//   updatePathFromRoot(v, val): updates the path betwen vertex v and
//     the root of its connected component with the lazy value val
//   updatePath(v, w, val): updates the path between vertices v and w with
//     the lazy value val
//   queryVertex(v): queries the vertex v
// In practice, constructor has a moderate constant,
//   update and query functions have a very small constant plus
//   the constants of R's update and query functions
// Time Complexity:
//   constructor: O(V) + time complexity of R's constructor
//   updatePathFromRoot, updatePath, queryVertex:
//     time complexity of update/query
// Memory Complexity: O(V) + memory complexity of R
// Tested:
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=102
template <class R, const bool VALUES_ON_EDGES>
struct InvertiblePathUpdatesVertexQueries {
  using Data = typename R::Data; using Lazy = typename R::Lazy;
  LCA<> lca; int V, ind; vector<int> par, pre, post, vert; R ops;
  bool connected(int v, int w) { return lca.connected(v, w); }
  void updatePathFromRoot(int v, const Lazy &val) {
    return ops.update(pre[lca.root[v]], pre[v], val);
  }
  void updatePath(int v, int w, const Lazy &val) {
    updatePathFromRoot(v, val); updatePathFromRoot(w, val);
    int u = lca.lca(v, w); Lazy inv = R::invLazy(val);
    if (VALUES_ON_EDGES) inv = R::mergeLazy(inv, inv);
    updatePathFromRoot(u, inv);
    if (!VALUES_ON_EDGES && par[u] != -1) updatePathFromRoot(par[u], inv);
  }
  Data queryVertex(int v) {
    Data ret = ops.query(pre[v]); if (post[v] + 1 <= post[lca.root[v]])
      ret = R::merge(ret, R::invData(ops.query(post[v] + 1)));
    return ret;
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
    } else for (int v : roots) if (par[v] == -1) dfs(G, v, -1);
    vector<Data> ret; ret.reserve(V);
    for (int i = 0; i < V; i++) ret.push_back(A[vert[i]]);
    for (int v = 0; v < V; v++) if (post[v] + 1 < V) {
      ret[post[v] + 1] = R::merge(ret[post[v] + 1], R::invData(A[v]));
    }
    return ret;
  }
  template <class Forest> InvertiblePathUpdatesVertexQueries(
      const Forest &G, const vector<Data> &A,
      const vector<int> &roots = vector<int>())
      : lca(G, roots), V(G.size()), ind(-1), par(V, -1),
        pre(V), post(V), vert(V), ops(reorder(G, A, roots)) {}
  template <class Forest> InvertiblePathUpdatesVertexQueries(
      const Forest &G, const vector<Data> &A, int rt)
      : InvertiblePathUpdatesVertexQueries(G, A, vector<int>{rt}) {}
};
