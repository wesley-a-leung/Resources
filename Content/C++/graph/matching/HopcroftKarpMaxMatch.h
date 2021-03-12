#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum matching (and minimum vertex cover) on
//   an unweighted bipartite graph
// The maximum independent set is any vertex not in the minimum vertex cover
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic undirected bipartite graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
//   color: a vector of booleans of size V indicating the color of each vertex
// Fields:
//   V: the number of vertices in the graph
//   cardinality: the cardinality of the maximum matching
//   mate: the other vertex in the matching, or -1 if unmatched
//   inCover: a vector of booleans indicating whether a vertex is in the
//    minimum vertex cover or not
// In practice, has a very small constant
// Time Complexity:
//   constructor: O((V + E) sqrt V)
// Memory Complexity: O(V)
// Tested:
//   https://www.spoj.com/problems/MATCHING/
//   https://judge.yosupo.jp/problem/bipartitematching
//   https://dmoj.ca/problem/coci19c6p4
struct HopcroftKarpMaxMatch {
  int V, cardinality;
  vector<int> mate, lvl, q, type0; vector<bool> color, inCover, vis;
  template <class BipartiteGraph> bool bfs(const BipartiteGraph &G) {
    int front = 0, back = 0; for (int v : type0) {
      if (mate[v] == -1) lvl[q[back++] = v] = 0;
      else lvl[v] = -1;
    }
    while (front < back) {
      int v = q[front++]; for (int w : G[v]) {
        if (mate[w] == -1) return true;
        else if (lvl[mate[w]] == -1) lvl[q[back++] = mate[w]] = lvl[v] + 1;
      }
    }
    return false;
  }
  template <class BipartiteGraph> bool dfs(const BipartiteGraph &G, int v) {
    for (int w : G[v])
      if (mate[w] == -1 || (lvl[mate[w]] == lvl[v] + 1 && dfs(G, mate[w]))) {
        mate[mate[v] = w] = v; return true;
      }
    lvl[v] = -1; return false;
  }
  template <class BipartiteGraph>
  void dfsVertexCover(const BipartiteGraph &G, int v) {
    if (vis[v]) return;
    vis[v] = true;
    for (int w : G[v]) if ((mate[v] == w) == color[v]) dfsVertexCover(G, w);
  }
  template <class BipartiteGraph>
  HopcroftKarpMaxMatch(const BipartiteGraph &G, const vector<bool> &color)
      : V(G.size()), cardinality(0), mate(V, -1), lvl(V), q(V),
        color(color), inCover(V, false), vis(V, false) {
    for (int v = 0; v < V; v++) if (!color[v]) type0.push_back(v);
    while (bfs(G)) for (int v : type0) if (mate[v] == -1 && dfs(G, v))
      cardinality++;
    for (int v : type0) if (mate[v] == -1) dfsVertexCover(G, v);
    for (int v = 0; v < V; v++) inCover[v] = vis[v] == color[v];
  }
};
