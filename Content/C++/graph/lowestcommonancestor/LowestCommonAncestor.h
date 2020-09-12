#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/FischerHeunStructure.h"
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a forest
//   and the distance between 2 vertices by reduing the problem to a
//   range minimum query using the Fischer Heun Structure
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the forest
// Constructor Arguments:
//   G: a generic forest data structure (weighted or unweighted)
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints for an unweighted forest, or a list of
//         pair<int, T> for a weighted forest with weights of type T)
//       size() const: returns the number of vertices in the forest
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
// Fields:
//   root: vector of roots for the forest each vertex is in
//   dep: vector of depths to each vertex from the root of
//     its connected component
//   pre: vector of the pre order traversal indices for each vertex
//   vert: vector of vertex for each pre order index
// Functions:
//   lca(v, w): returns the lowest common ancestor of vertices v and w assuming
//     v and w are connected
//   connected(v, w): returns true if and only if v and w are connected
//   dist(v, w): returns the distance between vertices v and w assuming
//     v and w are connected
// In practice, lca and dist have a moderate constant, constructor is
//   dependent on the forest data structure
// Time Complexity:
//   constructor: O(V)
//   lca, connected, dist: O(1)
// Memory Complexity: O(V)
// Tested:
//   https://judge.yosupo.jp/problem/lca
//   https://www.spoj.com/problems/LCASQ
//   https://codeforces.com/contest/1062/problem/E
//   https://dmoj.ca/problem/rte16s3
template <class T = int> struct LCA {
  using RMQ = FischerHeunStructure<int, greater<int>>;
  int V; vector<int> root, pre, vert; vector<T> dep; RMQ FHS;
  int getTo(int e) { return e; }
  T getWeight(int) { return 1; }
  int getTo(const pair<int, T> &e) { return e.first; }
  T getWeight(const pair<int, T> &e) { return e.second; }
  template <class Forest>
  void dfs(const Forest &G, int v, int prev, int r, T d) {
    root[v] = r; dep[v] = d; pre[v] = vert.size(); for (auto &&e : G[v]) {
      int w = getTo(e);
      if (w != prev) { vert.push_back(v); dfs(G, w, v, r, d + getWeight(e)); }
    }
  }
  template <class Forest> RMQ init(const Forest &G, const vector<int> &roots) {
    vert.reserve(V); if (roots.empty()) {
      for (int v = 0; v < V; v++) if (root[v] == -1) dfs(G, v, -1, v, T());
    } else for (int v : roots) if (root[v] == -1) dfs(G, v, -1, v, T());
    int i = 0; return RMQ(vert.size(), [&] { return pre[vert[i++]]; });
  }
  template <class Forest>
  LCA(const Forest &G, const vector<int> &roots = vector<int>())
      : V(G.size()), root(V, -1), pre(V), dep(V), FHS(init(G, roots)) {}
  template <class Forest> LCA(const Forest &G, int rt)
      : LCA(G, vector<int>{rt}) {}
  int lca(int v, int w) {
    if (v == w) return v;
    if (pre[v] > pre[w]) swap(v, w);
    return vert[FHS.query(pre[v], pre[w] - 1)];
  }
  bool connected(int v, int w) { return root[v] == root[w]; }
  T dist(int v, int w) { return dep[v] + dep[w] - 2 * dep[lca(v, w)]; }
};
