#pragma once
#include <bits/stdc++.h>
using namespace std;

// Decomposes an undirected graph into 2-edge connected components and
//   identifies bridges
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic graph structure
//     Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Fields:
//   id: a vector of the index of the 2-edge connected component each vertex
//     is part of
//   components: a vector of vectors containing the vertices in each 2-edge
//     connected component
//   bridges: a vector of pairs that stores the bridges in the graph
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
// Memory Complexity: O(V)
// Tested:
//   Stress Tested
//   https://judge.yosupo.jp/problem/two_edge_connected_components
struct Bridges {
  int ind, top; vector<int> id, low, pre, stk; vector<vector<int>> components;
  vector<pair<int, int>> bridges;
  void makeComponent(int v) {
    int w; components.emplace_back(); do {
      id[w = stk[--top]] = components.size() - 1;
      components.back().push_back(w);
    } while (w != v);
  }
  template <class Graph> void dfs(const Graph &G, int v, int prev) {
    id[stk[top++] = v] = -1; low[v] = pre[v] = ind++; bool parEdge = false;
    for (int w : G[v]) {
      if (w == prev && !parEdge) { parEdge = true; continue; }
      if (id[w] == -2) {
        dfs(G, w, v); low[v] = min(low[v], low[w]);
        if (low[w] == pre[w]) { bridges.emplace_back(v, w); makeComponent(w); }
      } else if (id[w] == -1) low[v] = min(low[v], pre[w]);
    }
  }
  template <class Graph> Bridges(const Graph &G)
      : ind(0), top(0), id(G.size(), -2), low(G.size()), pre(G.size()),
        stk(G.size()) {
    for (int v = 0; v < int(G.size()); v++) if (id[v] == -2) {
      dfs(G, v, -1); if (top > 0) makeComponent(v);
    }
  }
};
