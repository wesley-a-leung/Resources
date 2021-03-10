#pragma once
#include <bits/stdc++.h>
using namespace std;

// Multidirectional breadth first search of an undirected, unweighted graph to
//   query for the closest distance between two distinct vertices in a set
//   of vertices
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
//   closest(src): returns the closest distance between any two distinct
//     vertices in src
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
//   closest: O(V + E) worst case, much better in practice if two source
//     vertices are connected and the graph is random and sparse
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/acc1p2
//   https://dmoj.ca/problem/wac1p6
template <class Graph> struct MultidirectionalBFS {
  Graph G; vector<int> d, from, vis, q; int stamp;
  MultidirectionalBFS(const Graph &G)
      : G(G), d(G.size(), INT_MAX), from(G.size(), -1), vis(G.size(), 0),
        q(G.size()), stamp(0) {}
  int closest(const vector<int> &srcs) {
    assert(int(srcs.size()) >= 2);
    int front = 0, back = 0; ++stamp; for (int s : srcs) {
      if (vis[s] == stamp) return 0;
      vis[q[back++] = from[s] = s] = stamp; d[s] = 0;
    }
    int mn = INT_MAX; while (front < back) {
      int v = q[front++]; for (int w : G[v]) {
        if (vis[v] != vis[w]) {
          d[q[back++] = w] = d[v] + 1; vis[w] = vis[v]; from[w] = from[v];
        } else if (from[v] != from[w]) {
          int sm = d[v] + d[w] + 1; if (sm > mn) return mn;
          mn = sm;
        }
      }
    }
    return mn;
  }
};
