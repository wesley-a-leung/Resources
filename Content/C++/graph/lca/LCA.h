#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/FischerHeunStructure.h"
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a forest
//   and the distance between 2 vertices by reduing the problem to a
//   range minimum query using the Fischer Heun Structure
// Vertices are 0-indexed
// In practice, lca and getDist have a moderate constant, constructor is
//   dependent on the tree data structure
// contructor accepts a generic tree data structure (weighted or unweighted)
//   with the [] operator (const) defined to iterate over the adjacency list
//   (which is a list of ints for an unweighted tree, or a list of pair<int, T>
//   for a weighted tree with weights of type T), as well as a member function
//   size() (const) that returns the number of vertices in the graph, and a
//   single root of the tree, or a list of roots of the forests (if no root is
//   provided, then the minimum vertex is chosen for each forest)
// getDist assumes v and w are connected
// Time Complexity:
//   constructor: O(V)
//   lca, connected, getDist: O(1)
// Memory Complexity: O(V)
// Tested:
//   https://judge.yosupo.jp/problem/lca
//   https://www.spoj.com/problems/LCASQ
//   https://codeforces.com/contest/1062/problem/E
//   https://dmoj.ca/problem/rte16s3
template <class T = int> struct LCA {
  using RMQ = FischerHeunStructure<int, function<bool(int, int)>>;
  int V, ind; vector<int> root, pre, vert; vector<T> dist; RMQ FHS;
  int getTo(int e) { return e; }
  T getWeight(int) { return 1; }
  int getTo(const pair<int, T> &e) { return e.first; }
  T getWeight(const pair<int, T> &e) { return e.second; }
  template <class Tree> void dfs(const Tree &G, int v, int prev, int r, T d) {
    root[v] = r; dist[v] = d; vert[pre[v] = ind++] = v; for (auto &&e : G[v]) {
      int w = getTo(e);
      if (w != prev) { dfs(G, w, v, r, d + getWeight(e)); vert[ind++] = v; }
    }
  }
  template <class Tree> RMQ init(const Tree &G, const vector<int> &roots) {
    ind = 0; if (roots.empty()) {
      for (int v = 0; v < V; v++) if (root[v] == -1) dfs(G, v, -1, v, T());
    } else for (int rt : roots) dfs(G, rt, -1, rt, T());
    return RMQ(vert.begin(), vert.begin() + ind, [&] (int v, int w) {
                 return dist[v] > dist[w];
               });
  }
  template <class Tree>
  LCA(const Tree &G, const vector<int> &roots = vector<int>())
      : V(G.size()), root(V, -1), pre(V), vert(max(0, V * 2 - 1)), dist(V),
        FHS(init(G, roots)) {}
  template <class Tree> LCA(const Tree &G, int rt)
      : LCA(G, vector<int>(1, rt)) {}
  int lca(int v, int w) {
    if (pre[v] > pre[w]) swap(v, w);
    return FHS.query(pre[v], pre[w]);
  }
  bool connected(int v, int w) { return root[v] == root[w]; }
  T getDist(int v, int w) { return dist[v] + dist[w] - 2 * dist[lca(v, w)]; }
};
