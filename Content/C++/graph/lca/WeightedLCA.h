#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/FischerHeunStructure.h"
using namespace std;

// Supports queries for the lowest common ancestor of 2 vertices in a weighted
//   forest and the distance between 2 vertices by reduing the problem to a
//   range minimum query using the Fischer Heun Structure
// Vertices are 0-indexed
// In practice, lca and getDist have a moderate constant, constructor is
//   dependent on the weighted graph data structure
// contructor accepts a generic weighted graph data structure with the
//   [] operator defined to iterate over the adjacency list, and a single root
//   of the tree, or a list of roots of the forests (if no root is provided,
//   then the minimum vertex is chosen for each forest)
// getDist assumes v and w are connected
// Time Complexity:
//   constructor: O(V)
//   lca, connected, getDist: O(1)
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/rte16s3
template <class T> struct WeightedLCA {
  using RMQ = FischerHeunStructure<int, function<bool(int, int)>>;
  int ind; vector<int> root, pre, vert; vector<T> dist; RMQ FHS;
  template <class WeightedGraph>
  void dfs(const WeightedGraph &G, int v, int prev, int r, T d) {
    root[v] = r; dist[v] = d; vert[pre[v] = ind++] = v;
    for (auto &&e : G[v]) if (e.first != prev) {
      dfs(G, e.first, v, r, d + e.second); vert[ind++] = v;
    }
  }
  template <class WeightedGraph>
  RMQ init(const WeightedGraph &G, const vector<int> &roots) {
    ind = 0; if (roots.empty()) {
      for (int v = 0; v < G.V; v++) if (root[v] == -1) dfs(G, v, -1, v, T());
    } else for (int rt : roots) dfs(G, rt, -1, rt, T());
    return move(RMQ(vert.begin(), vert.begin() + ind, [&] (int v, int w) {
                      return dist[v] > dist[w];
                    }));
  }
  template <class WeightedGraph>
  WeightedLCA(const WeightedGraph &G, const vector<int> &roots = vector<int>())
      : root(G.V, -1), pre(G.V), vert(max(0, G.V * 2 - 1)), dist(G.V),
        FHS(init(G, roots)) {}
  template <class WeightedGraph> WeightedLCA(const WeightedGraph &G, int rt)
      : WeightedLCA(G, vector<int>(1, rt)) {}
  int lca(int v, int w) {
    if (pre[v] > pre[w]) swap(v, w);
    return FHS.query(pre[v], pre[w]);
  }
  bool connected(int v, int w) { return root[v] == root[w]; }
  T getDist(int v, int w) { return dist[v] + dist[w] - 2 * dist[lca(v, w)]; }
};
