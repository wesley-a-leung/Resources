#pragma once
#include <bits/stdc++.h>
using namespace std;

// Cuthill-McKee reordering of a graph to reduce the bandwith
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic undirected graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
//   rev: a boolean indicating whether the order should be reversed or not,
//     performs better with Gaussian Elimination
// Fields:
//   mapping: a mapping from the original vertex to the reordered vertex
//   revMapping: a mapping from the reordered vertex to the original vertex
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V log V + E log E)
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/ddrp3
struct CuthillMcKee {
  int V; vector<int> mapping, revMapping, deg;
  template <class Graph> CuthillMcKee(const Graph &G, bool rev = false)
      : V(G.size()), mapping(V, 0), revMapping(V), deg(V, 0) {
    for (int v = 0; v < V; v++) for (int w : G[v]) { deg[v]++; deg[w]++; }
    auto cmpDeg = [&] (int v, int w) {
      return make_pair(deg[v], v) < make_pair(deg[w], w);
    };
    int front = 0, back = 0; vector<int> P(V); iota(P.begin(), P.end(), 0);
    sort(P.begin(), P.end(), cmpDeg); for (int i = 0; i < V; i++) {
      int s = P[i]; if (mapping[s]) continue;
      mapping[revMapping[back++] = s] = 1; while (front < back) {
        int v = revMapping[front++]; vector<int> adj; adj.reserve(deg[v]);
        for (int w : G[v]) adj.push_back(w);
        sort(adj.begin(), adj.end(), cmpDeg);
        for (int w : adj) if (!mapping[w]) mapping[revMapping[back++] = w] = 1;
      }
    }
    if (rev) reverse(revMapping.begin(), revMapping.end());
    for (int i = 0; i < V; i++) mapping[revMapping[i]] = i;
  }
};
