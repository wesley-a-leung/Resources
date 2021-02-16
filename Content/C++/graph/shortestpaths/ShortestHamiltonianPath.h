#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the shortest Hamiltonian path
// All vertices are visited exactly once, does not return to the start 
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   matrix: a V x V matrix containing the minimum weight of a directed edge
//     between two vertices, INF if an edge doesn't exist
// Fields:
//   shortestPathDist: the distance of the shortest Hamiltonian path, INF if
//     no path exists (V <= 1 implies shortestPathDist == 0)
//   ord: the order the vertices are visited in the shortest Hamiltonian path
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V^2 2^V)
// Memory Complexity: O(V 2^V)
// Tested:
//   Fuzz Tested
template <class T> struct ShortestHamiltonianPath {
  vector<vector<T>> dp; T INF, shortestPathDist; vector<int> ord;
  ShortestHamiltonianPath(const vector<vector<T>> &matrix,
                          T INF = numeric_limits<T>::max())
      : dp(1 << matrix.size(), vector<T>(matrix.size(), INF)), INF(INF),
        shortestPathDist(matrix.empty() ? T() : INF), ord(matrix.size(), 0) {
    int V = matrix.size(); for (int i = 0; i < V; i++) dp[1 << i][i] = T();
    for (int mask = 0; mask < (1 << V); mask++) for (int i = 0; i < V; i++)
      if ((mask >> i) & 1) {
        for (int o = mask ^ (1 << i), j = 0; j < V; j++) if ((mask >> j) & 1) {
          if (matrix[j][i] < INF && dp[o][j] < INF
              && dp[mask][i] > dp[o][j] + matrix[j][i])
            dp[mask][i] = dp[o][j] + matrix[j][i];
        }
      }
    int cur = (1 << V) - 1; for (int i = 0; i < V; i++)
      shortestPathDist = min(shortestPathDist, dp.back()[i]);
    if (shortestPathDist >= INF) return;
    auto get = [&] (int mask, int j, int last) {
      T weight = (last == -1 ? T() : matrix[j][last]);
      if (dp[mask][j] < INF && weight < INF) return dp[mask][j] + weight;
      else return INF;
    };
    for (int last = -1, i = V - 1; i >= 0; i--) {
      int bj = -1; for (int j = 0; j < V; j++) if ((cur >> j) & 1)
        if (bj == -1 || get(cur, bj, last) > get(cur, j, last)) bj = j;
      cur ^= 1 << (last = ord[i] = bj);
    }
  }
};
