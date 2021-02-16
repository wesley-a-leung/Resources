#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the shortest Hamiltonian cycle
// All vertices are visited exactly once before returning to the start vertex
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   matrix: a V x V matrix containing the minimum weight of a directed edge
//     between two vertices, INF if an edge doesn't exist
// Fields:
//   shortestCycleDist: the distance of the shortest Hamiltonian cycle, INF if
//     no cycle exists (V == 0 implies shortestCycleDist == 0,
//     V == 1 implies shortestCycleDist == INF)
//   ord: the order the vertices are visited in the shortest Hamiltonian cycle,
//     the start vertex is not repeated
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V^2 2^V)
// Memory Complexity: O(V 2^V)
// Tested:
//   Fuzz Tested
template <class T> struct ShortestHamiltonianCycle {
  vector<vector<T>> dp; T INF, shortestCycleDist; vector<int> ord;
  ShortestHamiltonianCycle(const vector<vector<T>> &matrix,
                           T INF = numeric_limits<T>::max())
      : dp(1 << matrix.size(), vector<T>(matrix.size(), INF)), INF(INF),
        shortestCycleDist(matrix.empty() ? T() : INF), ord(matrix.size(), 0) {
    int V = matrix.size(); if (V > 0) dp[1][0] = T();
    for (int mask = 1; mask < (1 << V); mask += 2) for (int i = 1; i < V; i++)
      if ((mask >> i) & 1) {
        for (int o = mask ^ (1 << i), j = 0; j < V; j++) if ((mask >> j) & 1) {
          if (matrix[j][i] < INF && dp[o][j] < INF
              && dp[mask][i] > dp[o][j] + matrix[j][i])
            dp[mask][i] = dp[o][j] + matrix[j][i];
        }
      }
    int cur = (1 << V) - 1;
    for (int i = 1; i < V; i++) if (dp.back()[i] < INF && matrix[i][0] < INF)
      shortestCycleDist = min(shortestCycleDist, dp.back()[i] + matrix[i][0]);
    if (shortestCycleDist >= INF) return;
    auto get = [&] (int mask, int j, int last) {
      if (dp[mask][j] >= INF || matrix[j][last] >= INF) return INF;
      return dp[mask][j] + matrix[j][last];
    };
    for (int last = 0, i = V - 2; i >= 0; i--) {
      int bj = -1; for (int j = 1; j < V; j++) if ((cur >> j) & 1)
        if (bj == -1 || get(cur, bj, last) > get(cur, j, last)) bj = j;
      cur ^= 1 << (last = ord[i] = bj);
    }
  }
};
