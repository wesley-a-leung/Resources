#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bidirectional breadth first search of an undirected, unweighted graph to
//   query for the distance between two vertices
// Vertices are 0-indexed
// Template Arguments:
//   Graph: a generic undirected graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         which is a list of ints
//       size() const: returns the number of vertices in the graph
// Constructor Arguments:
//   G: a instance of Graph
// Functions:
//   dist(s, t): returns the distance between vertices s and t
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
//   dist: O(V + E) worst case, much better in practice if s and t are
//     connected and the graph is random and sparse
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/acc1p2
template <class Graph> struct BidirectionalBFS {
  Graph G; vector<int> d, vis, q; int stamp;
  BidirectionalBFS(const Graph &G)
      : G(G), d(G.size(), INT_MAX), vis(G.size(), 0), q(G.size()), stamp(0) {}
  int dist(int s, int t) {
    if (s == t) return 0;
    int front = 0, back = 0; d[s] = d[t] = 0;
    vis[q[back++] = s] = ++stamp; vis[q[back++] = t] = -stamp;
    while (front < back) {
      int v = q[front++]; for (int w : G[v]) {
        if (vis[v] == -vis[w]) return d[v] + d[w] + 1;
        else if (vis[v] != vis[w]) {
          d[q[back++] = w] = d[v] + 1; vis[w] = vis[v];
        }
      }
    }
    return INT_MAX;
  }
};
