#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum spanning tree (or forest) using Classical Prims's
//   algorithm
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic weighted graph structure
//     with the [] operator (const) defined to iterate over the adjacency list
//     (which is a list of pair<int, T> with weights of type T), as well as a
//     member function size() (const) that returns the number of vertices
//     in the graph
//   INF: a value for infinity
// Fields:
//   mstWeight: the weight of the mst
//   mstEdges: a vector of tuples of the edges in the mst
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V^2 + E)
// Memory Complexity: O(V + E)
// Tested:
//   Stress Tested
//   https://mcpt.ca/problem/anmstproblem
template <class T> struct ClassicalPrimMST {
  using Edge = tuple<int, int, T>; T mstWeight; vector<Edge> mstEdges;
  template <class WeightedGraph>
  ClassicalPrimMST(const WeightedGraph &G, T INF = numeric_limits<T>::max())
      : mstWeight() {
    int V = G.size(); vector<bool> done(V, false); vector<T> mn(V, INF);
    vector<int> to(V, -1); for (int s = 0; s < V; s++) if (!done[s]) {
      mn[s] = T(); while (true) {
        int v = -1; for (int w = 0; w < V; w++)
          if (!done[w] && (v == -1 || mn[v] > mn[w])) v = w;
        if (v == -1 || mn[v] >= INF) break;
        done[v] = true;
        for (auto &&e : G[v]) if (!done[e.first] && e.second < mn[e.first]) {
          to[e.first] = v; mn[e.first] = e.second;
        }
      }
    }
    for (int v = 0; v < V; v++) if (to[v] != -1) {
      mstEdges.emplace_back(v, to[v], mn[v]); mstWeight += mn[v];
    }
  }
};
