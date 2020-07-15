#pragma once
#include <bits/stdc++.h>
#include "../components/StronglyConnectedComponents.h"
using namespace std;

// Computes the Transitive Closure in a graph using strongly connected
//   components and dynamic programming with bitset optimizations
// Template Arguments:
//   MAXV: the maximum number of vertices in the graph
// Constructor Arguments:
//   G: a generic graph data structure
//     with the [] operator (const) defined to iterate over the adjacency list
//     (which is a list of ints), as well as a member function size() (const)
//     that returns the number of vertices in the graph
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
//   https://ecna18.kattis.com/problems/watchyourstep
template <const int MAXV> struct TransitiveClosureSCC {
  vector<pair<int, int>> DAG; SCC scc; vector<bool> vis; vector<int> st;
  vector<bitset<MAXV>> dp;
  void dfs(int v) {
    dp[v][v] = vis[v] = 1; for (int e = st[v]; e < st[v + 1]; e++) {
      int w = DAG[e].second; if (!vis[w]) dfs(w);
      dp[v] |= dp[w];
    }
  }
  template <class Graph> TransitiveClosureSCC(const Graph &G)
      : DAG(), scc(G, DAG), vis(scc.components.size(), false),
        st(vis.size() + 1, 0), dp(vis.size()) {
    for (int e = int(DAG.size()) - 1; e >= 0; e--) st[DAG[e].first + 1]++;
    partial_sum(st.begin(), st.end(), st.begin());
    for (int v = 0; v < int(dp.size()); v++) if (!vis[v]) dfs(v);
  }
  bool reachable(int v, int w) { return dp[scc.id[v]][scc.id[w]]; }
};

// Computes the Transitive Closure in a graph using Floyd Warshall
//   with bitset optimizations
// Template Arguments:
//   MAXV: the maximum number of vertices in the graph
// Constructor Arguments:
//   matrix: a V x V matrix (represented by V bitsets of size MAXV) such that
//     matrix[v][w] is 1 if there is a directed edge from v to w and
//       0 otherwise
// Functions:
//   reachable(v, w): returns true if w is reachable from v and false otherwise
// In practice, has a very small constant, slower than the SCC variant
// Time Complexity:
//   constructor: O(V^3 / 64)
//   reachable: O(1)
// Memory Complexity: O(MAXV V / 64)
// Tested:
//   Stress Tested
//   https://dmoj.ca/problem/acc2p2
//   https://ecna18.kattis.com/problems/watchyourstep
template <const int MAXV> struct TransitiveClosureFloydWarshall {
  vector<bitset<MAXV>> dp;
  TransitiveClosureFloydWarshall(const vector<bitset<MAXV>> &matrix)
      : dp(matrix) {
    int V = dp.size(); for (int v = 0; v < V; v++) dp[v][v] = 1;
    for (int u = 0; u < V; u++) for (int v = 0; v < V; v++)
      if (dp[v][u]) dp[v] |= dp[u];
  }
  bool reachable(int v, int w) { return dp[v][w]; }
};
