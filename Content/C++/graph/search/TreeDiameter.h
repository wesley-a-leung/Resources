#pragma once
#include <bits/stdc++.h>
#include "BreadthFirstSearch.h"
using namespace std;

// Computes the diameter of a tree
// Vertices are 0-indexed
// contructor accepts a generic tree data structure (weighted or unweighted)
//   with the [] operator (const) defined to iterate over the adjacency list
//   (which is a list of ints for an unweighted tree, or a list of
//   pair<int, T> for a weighted tree with weights of type T), as well as a
//   member function size() (const) that returns the number of vertices in the
//   tree
// getPath() returns the list of edges on the diameter
// In practice, constructor has a moderate constant
// Time Complexity:
//   constructor: O(V)
//   getPath: O(V)
// Memory Complexity: O(V)
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
