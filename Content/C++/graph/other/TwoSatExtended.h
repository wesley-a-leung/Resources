#pragma once
#include <bits/stdc++.h>
#include "../components/StronglyConnectedComponents.h"
using namespace std;

// Solves the two satisfiability problem and provides all possible values
//   for each variable
// Given an implication graph, determine whether a consistent assignment exists
// Functions for created an implication graph can be seen in ImplicationGraph.h
// Variables are 0-indexed
// Template Arguments:
//   MAXN: the maximum number of variables
// Constructor Arguments:
//   G: the implication graph with N * 2 vertices for N variables with vertex
//       a * 2 + 1 representing the affirmative for variable a, and a * 2
//       representing the negative for variable a
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Fields:
//   scc: the SCC of the implication graph
//   possibilities: a vector of integers representing the possible values for
//     that variable; 0 if guaranteed to be false, 1 if guaranteed to be true,
//     2 otherwise
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N + M + MAXN M / 32) for M equations
// Memory Complexity: O(N + M + MAXN M / 32)
// Tested:
//   https://dmoj.ca/problem/wac1p5
template <const int MAXN> struct TwoSatExtended {
  vector<pair<int, int>> DAG; SCC scc; vector<bitset<MAXN * 2>> dp;
  vector<int> possibilities;
  template <class ImplicationGraph>
  TwoSatExtended(const ImplicationGraph &G) : DAG(), scc(G, DAG) {
    int N = G.size() / 2;
    for (int i = 0; i < N; i++) if (scc.id[i * 2] == scc.id[i * 2 + 1]) return;
    dp.resize(scc.components.size());
    for (int i = 0; i < int(dp.size()); i++) dp[i][i] = 1;
    for (auto &&e : DAG) dp[e.first] |= dp[e.second];
    possibilities.assign(N, 2); for (int i = 0; i < N; i++) {
      if (dp[scc.id[i * 2]][scc.id[i * 2 + 1]]) possibilities[i] = 1;
      if (dp[scc.id[i * 2 + 1]][scc.id[i * 2]]) possibilities[i] = 0;
    }
  }
};
