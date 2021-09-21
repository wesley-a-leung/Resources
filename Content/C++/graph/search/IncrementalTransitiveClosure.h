#pragma once
#include <bits/stdc++.h>
using namespace std;

// Maintains the Transitive Closure in a graph after edges are added
// Constructor Arguments:
//   V: the number of vertices in the graph
// Functions:
//   reachable(v, w): returns true if w is reachable from v and false otherwise
//   addEdge(v, w): adds an edge between vertices v and w, returns true if the
//     resulting edge does not create a new cycle and false otherwise
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V^2)
//   reachable: O(1)
//   addEdge: O(V) amortized
// Memory Complexity: O(V^2)
// Tested:
//   https://www.spoj.com/problems/GHOSTS/
struct IncrementalTransitiveClosure {
  int V; vector<vector<int>> par; vector<vector<vector<int>>> ch;
  IncrementalTransitiveClosure(int V)
      : V(V), par(V, vector<int>(V, -1)), ch(V, vector<vector<int>>(V)) {}
  bool reachable(int v, int w) { return v == w || par[v][w] >= 0; }
  void meld(int root, int sub, int v, int w) {
    par[root][w] = v; ch[root][v].push_back(w);
    for (int c : ch[sub][w]) if (!reachable(root, c)) meld(root, sub, w, c);
  }
  bool addEdge(int v, int w) {
    if (reachable(w, v)) return false;
    if (reachable(v, w)) return true;
    for (int u = 0; u < V; u++) if (reachable(u, v) && !reachable(u, w))
      meld(u, w, v, w);
    return true;
  }
};
