#pragma once
#include <bits/stdc++.h>
using namespace std;

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
//   https://open.kattis.com/problems/watchyourstep
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
