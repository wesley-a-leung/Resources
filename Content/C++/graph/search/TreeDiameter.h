#pragma once
#include <bits/stdc++.h>
#include "BreadthFirstSearch.h"
using namespace std;

// Computes the diameter of a tree (weighted or unweighted)
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   G: a generic graph structure (weighted or unweighted)
//     Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints for an unweighted graph, or a list of
//         pair<int, T> for a weighted graph with weights of type T)
//       size() const: returns the number of vertices in the graph
//   INF: a value for infinity
// Fields:
//   endpoints: a pair containing the vertices on the endpoints of the diameter
//   diameter: the length of the diameter
// Functions:
//   getPath(): returns the list of edges on the diameter
// In practice, the constructor has a moderate constant
// Time Complexity:
//   constructor: O(V)
//   getPath: O(V)
// Memory Complexity: O(V)
// Tested:
//   https://judge.yosupo.jp/problem/tree_diameter
template <class T = int> struct TreeDiameter {
  BFS<T> bfs; pair<int, int> endpoints; T diameter;
  template <class Tree>
  TreeDiameter(const Tree &G, T INF = numeric_limits<T>::max())
      : bfs(G, 0, INF) {
    endpoints.first = max_element(bfs.dist.begin(), bfs.dist.end())
        - bfs.dist.begin();
    move(bfs); bfs = BFS<T>(G, endpoints.first);
    endpoints.second = max_element(bfs.dist.begin(), bfs.dist.end())
        - bfs.dist.begin();
    diameter = bfs.dist[endpoints.second];
  }
  vector<typename BFS<T>::Edge> getPath() {
    return bfs.getPath(endpoints.second);
  }
};
