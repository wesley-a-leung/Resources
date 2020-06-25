#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/FischerHeunStructure.h"
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a forest
//   and the distance between 2 vertices by reduing the problem to a
//   range minimum query using the Fischer Heun Structure
// Vertices are 0-indexed
// In practice, lca and getDist have a moderate constant, constructor is
//   dependent on the graph data structure
// contructor accepts a generic graph data structure with the [] operator
//   defined to iterate over the adjacency list, and a single root of the tree,
//   or a list of roots of the forests (if no root is provided, then the
//   minimum vertex is chosen for each forest)
// getDist assumes v and w are connected
// Time Complexity:
//   constructor: O(V)
//   lca, connected, getDist: O(1)
// Memory Complexity: O(V)
// Tested:
//   https://judge.yosupo.jp/problem/lca
//   https://www.spoj.com/problems/LCASQ
struct LCA {
  using RMQ = FischerHeunStructure<int, function<bool(int, int)>>;
  int ind; vector<int> root, pre, vert, dep; RMQ FHS;
  template <class Graph>
  void dfs(const Graph &G, int v, int prev, int r, int d) {
    root[v] = r; dep[v] = d; vert[pre[v] = ind++] = v; for (int w : G[v])
      if (w != prev) { dfs(G, w, v, r, d + 1); vert[ind++] = v; }
  }
  template <class Graph> RMQ init(const Graph &G, const vector<int> &roots) {
    ind = 0; if (roots.empty()) {
      for (int v = 0; v < G.V; v++) if (root[v] == -1) dfs(G, v, -1, v, 0);
    } else for (int rt : roots) dfs(G, rt, -1, rt, 0);
    return RMQ(vert.begin(), vert.begin() + ind, [&] (int v, int w) {
                 return dep[v] > dep[w];
               });
  }
  template <class Graph>
  LCA(const Graph &G, const vector<int> &roots = vector<int>())
      : root(G.V, -1), pre(G.V), vert(max(0, G.V * 2 - 1)), dep(G.V),
        FHS(init(G, roots)) {}
  template <class Graph> LCA(const Graph &G, int rt)
      : LCA(G, vector<int>(1, rt)) {}
  int lca(int v, int w) {
    if (pre[v] > pre[w]) swap(v, w);
    return FHS.query(pre[v], pre[w]);
  }
  bool connected(int v, int w) { return root[v] == root[w]; }
  int getDist(int v, int w) { return dep[v] + dep[w] - 2 * dep[lca(v, w)]; }
};
