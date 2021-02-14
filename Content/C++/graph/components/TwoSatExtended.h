#pragma once
#include <bits/stdc++.h>
#include "StronglyConnectedComponents.h"
using namespace std;

// Solves the two satisfiability problem and provides all possible values
//   for each variable
// Given a conjuntive normal form (x0 | x1) & (x2 | !x1) & ...
//   determine whether a consistent assignment exists
// Variables are 0-indexed
// Template Arguments:
//   MAXN: the maximum number of variables
// Constructor Arguments:
//   N: the number of variables
//   disjunctions: a vector of tuple in the form
//     (bool affI, int i, bool affJ, int j) each representing a disjunction
//     with affI indicating if x_i is the affirmative (if true) or the negation
//     (if false), and similarly with x_j
// Fields:
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
  vector<vector<int>> init(
      int N, const vector<tuple<bool, int, bool, int>> &disjunctions) {
    vector<vector<int>> G(N * 2); for (auto &&d : disjunctions) {
      int i = get<1>(d) * 2 + get<0>(d), j = get<3>(d) * 2 + get<2>(d);
      G[i ^ 1].push_back(j); G[j ^ 1].push_back(i);
    }
    return G;
  }
  TwoSatExtended(int N,
                 const vector<tuple<bool, int, bool, int>> &disjunctions)
      : DAG(), scc(init(N, disjunctions), DAG) {
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
