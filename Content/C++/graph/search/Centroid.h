#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the centroids for each connected component in a forest
// Vertices are 0-indexed
// Function Arguments:
//   G: a generic forest structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
// Return Value: a vector of vectors of integers containing the centroids for
//   each connected component with each connected component having either one
//   or two centroids; connected components are ordered by their minimum vertex
// In practice, has a moderate constant
// Time Complexity: O(V)
// Memory Complexity: O(V)
// Tested:
//   https://codeforces.com/contest/1406/problem/C
template <class Forest> vector<vector<int>> getCentroids(const Forest &G) {
  int V = G.size(); vector<int> size(V, 0); vector<vector<int>> centroids;
  function<int(int, int)> getSize = [&] (int v, int prev) {
    size[v] = 1; for (int w : G[v]) if (w != prev) size[v] += getSize(w, v);
    return size[v];
  };
  function<void(int, int, int)> dfs = [&] (int v, int prev, int compSize) {
    bool b = true; for (int w : G[v])
      if (w != prev) { dfs(w, v, compSize); b &= size[w] <= compSize / 2; }
    if (b && compSize - size[v] <= compSize / 2) centroids.back().push_back(v);
  };
  for (int v = 0; v < V; v++)
    if (size[v] == 0) { centroids.emplace_back(); dfs(v, -1, getSize(v, -1)); }
  return centroids;
}
