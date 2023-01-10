#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the biconnected components of an undirected graph
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic undirected graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
//   blockCutForestEdges: a reference to a vector of pairs that will store the
//     edges in the block-cut forest with the articulation vertices having the
//     same index in the original graph, non articulation vertices being
//     isolated, and each bcc/block having an index offset by V
// Fields:
//   ids: a vector of vectors of the indices of the bccs each vertex is part of
//   components: a vector of vectors containing the vertices in each bcc
//   articulation: a vector of booleans that indicates whether each vertex
//     is an articulation point or not
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
// Memory Complexity: O(V + E)
// Tested:
//   Stress Tested
//   https://judge.yosupo.jp/problem/biconnected_components
struct BCC {
  int ind; vector<int> low, pre; vector<pair<int, int>> stk;
  vector<bool> articulation; vector<vector<int>> ids, components;
  vector<vector<pair<int, int>>> edgesInComp;
  void assign(int x, int id) {
    if (ids[x].empty() || ids[x].back() != id) {
      ids[x].push_back(id); components.back().push_back(x);
    }
  }
  void makeComponent(int s) {
    int x, y, id = components.size(); components.emplace_back();
    edgesInComp.emplace_back(); while (int(stk.size()) > s) {
      tie(x, y) = stk.back(); stk.pop_back(); assign(x, id); assign(y, id);
      edgesInComp.back().emplace_back(x, y);
    }
  }
  template <class Graph> void dfs(const Graph &G, int v, int prev) {
    low[v] = pre[v] = ind++; bool parEdge = false; int deg = 0;
    for (int w : G[v]) {
      deg++; if (w == prev && !parEdge) parEdge = true;
      else if (pre[w] == -1) {
        int s = stk.size(); stk.emplace_back(v, w);
        dfs(G, w, v); low[v] = min(low[v], low[w]);
        if (low[w] >= pre[v]) { articulation[v] = true; makeComponent(s); }
      } else {
        low[v] = min(low[v], pre[w]);
        if (pre[w] < pre[v]) stk.emplace_back(v, w);
      }
    }
    if (deg == 0) { makeComponent(0); assign(v, int(components.size()) - 1); }
  }
  template <class Graph> BCC(const Graph &G)
      : ind(0), low(G.size()), pre(G.size(), -1),
        articulation(G.size(), false), ids(G.size()) {
    for (int v = 0; v < int(G.size()); v++) if (pre[v] == -1) dfs(G, v, -1);
  }
  template <class Graph>
  BCC(const Graph &G, vector<pair<int, int>> &blockCutForestEdges) : BCC(G) {
    for (int v = 0; v < int(G.size()); v++) if (articulation[v])
      for (int id : ids[v])
        blockCutForestEdges.emplace_back(v, int(G.size()) + id);
  }
};
