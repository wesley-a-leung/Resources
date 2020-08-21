#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the biconnected components of an undirected graph
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic graph data structure
//     Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
//   blockForestEdges: a reference to a vector of pairs that will store the
//     edges in the block forest with the articulation vertices having the
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
//   https://dmoj.ca/problem/ccoprep1p2
//   https://dmoj.ca/problem/dmpg18s6
struct BCC {
  int ind; vector<int> low, pre; vector<pair<int, int>> stk;
  vector<vector<int>> ids, components; vector<bool> articulation;
  void assign(int x, int id) {
    if (ids[x].empty() || ids[x].back() != id) {
      ids[x].push_back(id); components.back().push_back(x);
    }
  }
  void makeComponent(int v = -1, int w = -1) {
    int x, y, id = components.size(); components.emplace_back(); do {
      tie(x, y) = stk.back(); stk.pop_back(); assign(x, id); assign(y, id);
    } while (!stk.empty() && (x != v || y != w));
  }
  template <class Graph> void dfs(const Graph &G, int v, int prev) {
    low[v] = pre[v] = ind++; int children = 0; for (int w : G[v]) {
      if (pre[w] == -1) {
        children++; stk.emplace_back(v, w); dfs(G, w, v);
        low[v] = min(low[v], low[w]);
        if ((prev == -1 && children > 1) || (prev != -1 && low[w] >= pre[v])) {
          articulation[v] = true; makeComponent(v, w);
        }
      } else if (w != prev && pre[w] < low[v]) {
        low[v] = pre[w]; stk.emplace_back(v, w);
      }
    }
  }
  template <class Graph> BCC(const Graph &G)
      : ind(0), low(G.size()), pre(G.size(), -1),
        ids(G.size()), articulation(G.size(), false) {
    for (int v = 0; v < int(G.size()); v++) if (pre[v] == -1) {
      dfs(G, v, -1); if (!stk.empty()) makeComponent();
    }
  }
  template <class Graph>
  BCC(const Graph &G, vector<pair<int, int>> &blockForestEdges) : BCC(G) {
    for (int v = 0; v < int(G.size()); v++) if (articulation[v])
      for (int id : ids[v])
        blockForestEdges.emplace_back(v, int(G.size()) + id);
  }
};
