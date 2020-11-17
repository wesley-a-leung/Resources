#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum matching in a general undirected graph, such that each
//   vertex is incident with at most one edge in the matching
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic undirected graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Fields:
//   cardinality: the cardinality of the maximum matching
//   mate: the other vertex in the matching, or -1 if unmatched
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(V^2 E)
// Memory Complexity: O(V)
// Tested:
//   https://judge.yosupo.jp/problem/general_matching
//   https://codeforces.com/contest/1089/problem/B
//   https://uoj.ac/problem/79
struct EdmondsMaxMatch {
  int V, cardinality, stamp;
  vector<int> mate, par, id, vis2, q; vector<bool> vis, blossom;
  void markPath(int i, int b, int j) {
    for (; id[i] != b; i = par[mate[i]]) {
      blossom[id[i]] = blossom[id[mate[i]]] = true; par[i] = j; j = mate[i];
    }
  }
  int lca(int i, int j) {
    for (stamp++;; i = par[mate[i]]) {
      vis2[i = id[i]] = stamp; if (mate[i] == -1) break;
    }
    for (;; j = par[mate[j]]) if (vis2[j = id[j]] == stamp) return j;
  }
  template <class Graph> int getAugmentingPath(const Graph &G, int s) {
    fill(par.begin(), par.end(), -1); fill(vis.begin(), vis.end(), false);
    iota(id.begin(), id.end(), 0); vis[s] = true; int front = 0, back = 0;
    q[back++] = s; while (front < back) {
      int v = q[front++]; for (int w : G[v]) {
        if (id[v] == id[w] || mate[v] == w) continue;
        if (w == s || (mate[w] != -1 && par[mate[w]] != -1)) {
          int u = lca(v, w); fill(blossom.begin(), blossom.end(), false);
          markPath(v, u, w); markPath(w, u, v);
          for (int i = 0; i < V; i++) if (blossom[id[i]]) {
            id[i] = u; if (!vis[i]) vis[q[back++] = i] = true;
          }
        } else if (par[w] == -1) {
          par[w] = v; if (mate[w] == -1) return w;
          else if (!vis[mate[w]]) vis[q[back++] = mate[w]] = true;
        }
      }
    }
    return -1;
  }
  template <class Graph> EdmondsMaxMatch(const Graph &G)
      : V(G.size()), cardinality(0), stamp(0), mate(V, -1), par(V), id(V),
        vis2(V, 0), q(V), vis(V), blossom(V) {
    for (int s = 0; s < V; s++) if (mate[s] == -1)
      for (int v = getAugmentingPath(G, s); v != -1;) {
        int pv = par[v], ppv = mate[pv]; mate[mate[v] = pv] = v; v = ppv;
      }
    for (int v = 0; v < V; v++) cardinality += mate[v] != -1 && v < mate[v];
  }
};
