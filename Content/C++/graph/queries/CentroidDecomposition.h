#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the centroid of each component of a tree,
//   and recursively splits the component at that vertex
// Can be used to create a centroid tree, which has depth O(log V)
// Function Arguments:
//   G: a generic forest structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
//   f(G, excl, c, p): a function to call on each centroid of its
//     component where G is a reference to the graph, excl is a reference to
//     an array of bools that indicates whether a vertex has been previously
//     used as a centroid, c is the current centroid (excl[c] is false),
//     and p is its parent in the centorid tree
// Return Value: a vector of integers representing the parent of each vertex
//   in the centroid tree, or -1 if it is a root
// In practice, has a moderate constant
// Time Complexity: O(V log V)
// Memory Complexity: O(V)
// Tested:
//   https://codeforces.com/contest/321/problem/C
//   https://codeforces.com/contest/161/problem/D
template <class Forest, class F>
vector<int> centroidDecomposition(const Forest &G, F f) {
  int V = G.size(); vector<int> size(V), par(V, -1);
  vector<bool> excl(V, false);
  function<int(int, int)> getSize = [&] (int v, int prev) {
    size[v] = 1;
    for (int w : G[v]) if (w != prev && !excl[w]) size[v] += getSize(w, v);
    return size[v];
  };
  function<int(int, int, int)> dfs = [&] (int v, int prev, int compSize) {
    for (int w : G[v]) if (w != prev && !excl[w] && size[w] > compSize / 2)
      return dfs(w, v, compSize);
    return v;
  };
  vector<pair<int, int>> q(V); for (int s = 0; s < V; s++) if (par[s] == -1) {
    int front = 0, back = 0; q[back++] = make_pair(s, -1);
    while (front < back) {
      int v = q[front].first, c = dfs(v, -1, getSize(v, -1));
      par[c] = q[front++].second; f(G, excl, c, par[c]); excl[c] = true;
      for (int w : G[c]) if (!excl[w]) q[back++] = make_pair(w, c);
    }
  }
  return par;
}
