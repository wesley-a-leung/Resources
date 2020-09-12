#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a directed cycle in a directed graph (including self loops and
//   parallel edges)
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic directed graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Fields:
//   cycle: a vector of the vertices in the directed cycle with the first
//     vertex equal to the last vertex; if there is no cycle, then it is empty
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
// Memory Complexity: O(V)
// Tested:
//   Stress Tested
//   https://cses.fi/problemset/task/1678
struct DirectedCycle {
  int V; vector<bool> vis, onStk; vector<int> to, cycle;
  template <class Digraph> void dfs(const Digraph &G, int v) {
    vis[v] = onStk[v] = true; for (int w : G[v]) {
      if (!vis[w]) { to[w] = v; dfs(G, w); }
      else if (onStk[w]) {
        for (int x = v; x != w; x = to[x]) cycle.push_back(x);
        cycle.push_back(w); cycle.push_back(v);
        reverse(cycle.begin(), cycle.end());
      }
      if (!cycle.empty()) return;
    }
    onStk[v] = false;
  }
  template <class Digraph> DirectedCycle(const Digraph &G)
      : V(G.size()), vis(V, false), onStk(V, false), to(V) {
    for (int v = 0; v < V && cycle.empty(); v++) if (!vis[v]) dfs(G, v);
  }
};
