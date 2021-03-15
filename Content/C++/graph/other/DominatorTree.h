#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the dominator tree rooted at a vertex
// A vertex dominates v another vertex w if all paths from the root to w must
//   pass through v
// A vertex v is an immediate dominator of w if v dominates w and no other
//   vertex dominates w
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic directed graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Fields:
//   idom: a vector of the immediate dominator of each vertex, or -1 if it
//     is not connected to the root, or is the root vertex
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V + E log V)
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/dominatortree
struct DominatorTree {
  int V, ind; vector<int> idom, sdom, par, ord, U, UF, m;
  vector<vector<int>> bucket, H;
  int find(int v) {
    if (UF[v] == v) return v;
    int fv = find(UF[v]); if (sdom[m[v]] > sdom[m[UF[v]]]) m[v] = m[UF[v]];
    return UF[v] = fv;
  }
  int eval(int v) { find(v); return m[v]; }
  template <class Digraph> void dfs(const Digraph &G, int v) {
    ord[sdom[v] = ind++] = v;
    for (int w : G[v]) if (sdom[w] == -1) { par[w] = v; dfs(G, w); }
  }
  template <class Digraph> DominatorTree(const Digraph &G, int root)
      : V(G.size()), ind(0), idom(V, -1), sdom(V, -1), par(V, -1), ord(V, -1),
        U(V, -1), UF(V), m(V), bucket(V), H(V) {
    for (int v = 0; v < V; v++) {
      UF[v] = m[v] = v; for (int w : G[v]) H[w].push_back(v);
    }
    dfs(G, root); for (int i = ind - 1; i > 0; i--) {
      int w = ord[i]; for (int v : H[w]) if (sdom[v] >= 0)
        sdom[w] = min(sdom[w], sdom[eval(v)]);
      bucket[ord[sdom[w]]].push_back(w);
      for (int v : bucket[par[w]]) U[v] = eval(v);
      bucket[UF[w] = par[w]].clear();
    }
    for (int i = 1; i < ind; i++) {
      int w = ord[i], u = U[w]; idom[w] = sdom[sdom[w] == sdom[u] ? w : u];
    }
    for (int i = 1; i < ind; i++) { int w = ord[i]; idom[w] = ord[idom[w]]; }
  }
};
