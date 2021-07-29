#pragma once
#include <bits/stdc++.h>
using namespace std;

// Decomposes a tree into chains, such that a path from any vertex to the root
//   will cover at most O(log V) chains
// Can be used with PathQueries for path queries
// Vertices and indices are 0-indexed
// Constructor Arguments:
//   G: a generic forest data structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
// Fields:
//   root: vector of roots for the forest each vertex is in
//   dep: vector of depths to each vertex from the root of
//     its connected component
//   par: vector of parent vertices for each vertex (or -1 if its a root)
//   size: vector of sizes of the subtree for each vertex
//   head: vector of the head of the chain of each vertex
//   pre: vector of the pre order traversal indices for each vertex
//   post: vector of the post order traversal indices (the last pre order index
//     in its subtree) for each vertex
//   vert: vector of vertex for each pre order index
// Functions:
//   lca(v, w): returns the lowest common ancestor of vertices v and w assuming
//     v and w are connected
//   connected(v, w): returns true if and only if v and w are connected
//   dist(v, w): returns the distance between vertices v and w assuming
//     v and w are connected
//   kthParent(v, k): returns the kth parent of the vertex v
//   kthPath(v, w, k): returns the kth vertex on the path from v to w
// In practice, constructor has a moderate constant,
//   lca, dist, kthParent, kthPath, have a small constant
// Time Complexity:
//   constructor: O(V)
//   lca, dist, kthParent, kthPath: O(log V)
//   connected: O(1)
// Memory Complexity: O(V)
// Tested:
//   https://www.spoj.com/problems/QTREE2/
//   https://www.spoj.com/problems/GSS7/
//   https://judge.yosupo.jp/problem/vertex_set_path_composite
struct HLD {
  int V, ind; vector<int> root, dep, par, size, head, pre, post, vert;
  template <class Forest>
  void dfs(const Forest &G, int v, int prev, int r, int d) {
    root[v] = r; dep[v] = d; par[v] = prev; size[v] = 1; for (int w : G[v])
      if (w != prev) { dfs(G, w, v, r, d + 1); size[v] += size[w]; }
  }
  template <class Forest> void hld(const Forest &G, int v, int prev) {
    if (head[v] == -1) head[v] = v;
    vert[pre[v] = ++ind] = v; int heavy = -1;
    for (int w : G[v]) if (w != prev && (heavy == -1 || size[heavy] < size[w]))
      heavy = w;
    if (heavy != -1) { head[heavy] = head[v]; hld(G, heavy, v); }
    for (int w : G[v]) if (w != prev && w != heavy) hld(G, w, v);
    post[v] = ind;
  }
  int lca(int v, int w) {
    while (head[v] != head[w]) {
      if (dep[head[v]] < dep[head[w]]) w = par[head[w]];
      else v = par[head[v]];
    }
    return dep[v] < dep[w] ? v : w;
  }
  int dist(int v, int w) { return dep[v] + dep[w] - 2 * dep[lca(v, w)]; }
  int kthParent(int v, int k) {
    while (par[head[v]] != -1) {
      if (pre[v] - pre[head[v]] >= k) return vert[pre[v] - k];
      k -= pre[v] - pre[head[v]] + 1; v = par[head[v]];
    }
    return pre[v] < k ? -1 : vert[pre[v] - k];
  }
  int kthPath(int v, int w, int k) {
    int LCA = lca(v, w);
    if (dep[v] - dep[LCA] >= k) return kthParent(v, k);
    else return kthParent(w, dep[v] + dep[w] - 2 * dep[LCA] - k);
  }
  bool connected(int v, int w) { return root[v] == root[w]; }
  template <class Forest>
  HLD(const Forest &G, const vector<int> &roots = vector<int>())
      : V(G.size()), ind(-1), root(V, -1), dep(V), par(V), size(V),
        head(V, -1), pre(V), post(V), vert(V) {
    if (roots.empty()) {
      for (int v = 0; v < V; v++)
        if (root[v] == -1) { dfs(G, v, -1, v, 0); hld(G, v, -1); }
    } else for (int v : roots) { dfs(G, v, -1, v, 0); hld(G, v, -1); }
  }
  template <class Forest> HLD(const Forest &G, int rt)
      : HLD(G, vector<int>{rt}) {}
};
