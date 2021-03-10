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
//   par: vector of parent vertices for each vertex (or -1 if its a root)
//   pre: vector of the pre order traversal indices for each vertex
//   dep: vector of depths to each vertex from the root of
//     its connected component
// Functions:
//   lca(v, w): returns the lowest common ancestor of vertices v and w assuming
//     v and w are connected
//   getDirectChild(anc, des): returns the direct child of anc that is on the
//     path from anc to des, where anc is an ancestor of des
//   connected(v, w): returns true if and only if v and w are connected
//   dist(v, w): returns the distance between vertices v and w assuming
//     v and w are connected
// In practice, lca and dist have a moderate constant, constructor is
//   dependent on the forest data structure
// Time Complexity:
//   constructor: O(V)
//   lca, getDirectChild, connected, dist: O(1)
// Memory Complexity: O(V)
// Tested:
//   https://judge.yosupo.jp/problem/lca
//   https://www.spoj.com/problems/LCASQ
//   https://dmoj.ca/problem/rte16s3
//   https://dmoj.ca/problem/wac1p6
template <class T = int> struct LCA {
  using RMQ = FischerHeunStructure<int, greater_equal<int>>;
  int V, i; vector<int> root, par, pre, top, bot, stk; vector<T> dep; RMQ FHS;
  int getTo(int e) { return e; }
  T getWeight(int) { return 1; }
  int getTo(const pair<int, T> &e) { return e.first; }
  T getWeight(const pair<int, T> &e) { return e.second; }
  template <class Forest> void dfs(const Forest &G, int r) {
    int ssz = 0; stk[ssz++] = r; while (ssz > 0) {
      int v = stk[--ssz]; if (v != r) { top[i] = par[v]; bot[i++] = v; }
      root[v] = r; pre[v] = i; for (auto &&e : G[v]) {
        int w = getTo(e);
        if (w != par[v]) dep[stk[ssz++] = w] = dep[par[w] = v] + getWeight(e);
      }
    }
  }
  template <class Forest> RMQ init(const Forest &G, const vector<int> &roots) {
    if (roots.empty()) {
      for (int v = 0; v < V; v++) if (par[v] == -1) dfs(G, v);
    } else for (int v : roots) dfs(G, v);
    int j = 0; return RMQ(i, [&] { return pre[top[j++]]; });
  }
  template <class Forest>
  LCA(const Forest &G, const vector<int> &roots = vector<int>())
      : V(G.size()), i(0), root(V, -1), par(V, -1), pre(V, -1), top(V), bot(V),
        stk(V), dep(V, T()), FHS(init(G, roots)) {}
  template <class Forest> LCA(const Forest &G, int rt)
      : LCA(G, vector<int>{rt}) {}
  int lca(int v, int w) {
    if (v == w) return v;
    if (pre[v] > pre[w]) swap(v, w);
    return top[FHS.queryInd(pre[v], pre[w] - 1)];
  }
  int getDirectChild(int anc, int des) {
    return bot[FHS.queryInd(pre[anc], pre[des] - 1)];
  }
  bool connected(int v, int w) { return root[v] == root[w]; }
  T dist(int v, int w) { return dep[v] + dep[w] - 2 * dep[lca(v, w)]; }
};
