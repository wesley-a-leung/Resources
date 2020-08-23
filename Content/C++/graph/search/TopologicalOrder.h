#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the Topological Order of a directed acyclic graph
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic directed acyclic graph structure
//       which can be weighted or unweighted, though weights do not change
//       the topological order
//     Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints for an unweighted graph, or a list of
//         pair<int, T> for a weighted graph with weights of type T)
//       size() const: returns the number of vertices in the graph
// Fields:
//   ind: a vector of the topological order index for each vertex
//   ord: a vector of the vertices sorted by topological order index
// In practice, has a moderate constant, faster than DepthFirstOrder
// Time Complexity:
//   constructor: O(V + E)
// Memory Complexity: O(V)
// Tested:
//   https://atcoder.jp/contests/dp/tasks/dp_g
struct TopologicalOrder {
  int V; vector<int> ind, ord, inDeg;
  int getTo(int e) { return e; }
  template <class T> int getTo(const pair<int, T> &e) { return e.first; }
  template <class DAG> TopologicalOrder(const DAG &G)
      : V(G.size()), ind(V, -1), ord(V), inDeg(V, 0) {
    int front = 0, back = 0;
    for (int v = 0; v < V; v++) for (auto &&e : G[v]) inDeg[getTo(e)]++;
    for (int v = 0; v < V; v++) if (inDeg[v] == 0) ord[back++] = v;
    while (front < back) {
      int v = ord[front]; ind[v] = front++;
      for (auto &&e : G[v]) if (--inDeg[getTo(e)] == 0) ord[back++] = getTo(e);
    }
  }
};
