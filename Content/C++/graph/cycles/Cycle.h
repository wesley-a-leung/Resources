#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a cycle in an undirected graph (including self loops and
//   parallel edges)
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic graph structure
//     Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Fields:
//   cycle: a vector of the vertices in the cycle with the first vertex equal
//     to the last vertex; if there is no cycle, then it is empty
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
// Memory Complexity: O(V)
// Tested:
//   Stress Tested
//   https://cses.fi/problemset/task/1669
struct Cycle {
  int V; vector<bool> vis; vector<int> to, cycle;
  template <class Graph> void dfs(const Graph &G, int v, int prev) {
    vis[v] = true; for (int w : G[v]) {
      if (!vis[w]) dfs(G, w, to[w] = v);
      else if (w != prev) {
        for (int x = v; x != w; x = to[x]) cycle.push_back(x);
        cycle.push_back(w); cycle.push_back(v);
      }
      if (!cycle.empty()) return;
    }
  }
  template <class Graph>
  Cycle(const Graph &G) : V(G.size()), vis(V, false), to(V) {
    for (int v = 0; v < V; v++) {
      for (int w : G[v]) {
        if (v == w) { cycle.push_back(v); cycle.push_back(v); return; }
        if (vis[w]) {
          cycle.push_back(v); cycle.push_back(w); cycle.push_back(v); return;
        }
      }
      for (int w : G[v]) vis[w] = false;
    }
    fill(vis.begin(), vis.end(), false);
    for (int v = 0; v < V && cycle.empty(); v++) if (!vis[v]) dfs(G, v, -1);
  }
};
