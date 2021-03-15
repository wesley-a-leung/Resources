#pragma once
#include <bits/stdc++.h>
#include "../components/StronglyConnectedComponents.h"
using namespace std;

// Computes the Transitive Closure in a graph using strongly connected
//   components and dynamic programming with bitset optimizations
// Template Arguments:
//   MAXV: the maximum number of vertices in the graph
// Constructor Arguments:
//   G: a generic graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Functions:
//   reachable(v, w): returns true if w is reachable from v and false otherwise
// In practice, has a very small constant, faster than the
//   Floyd Warshall variant
// Time Complexity:
//   constructor: O(V + E + MAXV E / 64)
//   reachable: O(1)
// Memory Complexity: O(V + E + MAXV V / 64)
// Tested:
//   Stress Tested
//   https://dmoj.ca/problem/acc2p2
//   https://open.kattis.com/problems/watchyourstep
template <const int MAXV> struct TransitiveClosureSCC {
  vector<pair<int, int>> DAG; SCC scc; vector<bitset<MAXV>> dp;
  template <class Graph> TransitiveClosureSCC(const Graph &G)
      : DAG(), scc(G, DAG), dp(scc.components.size()) {
    for (int i = 0; i < int(dp.size()); i++) dp[i][i] = 1;
    for (auto &&e : DAG) dp[e.first] |= dp[e.second];
  }
  bool reachable(int v, int w) { return dp[scc.id[v]][scc.id[w]]; }
};
