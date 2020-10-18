#pragma once
#include <bits/stdc++.h>
#include "StronglyConnectedComponents.h"
using namespace std;

// Solves the two satisfiability problem
// Given a conjuntive normal form (x0 | x1) & (x2 | !x1) & ...
//   determine whether a consistent assignment exists
// Variables are 0-indexed
// Constructor Arguments:
//   N: the number of variables
//   disjunctions: a vector of tuple in the form
//     (bool affI, int i, bool affJ, int j) each representing a disjunction
//     with affI indicating if x_i is the affirmative (if true) or the negation
//     (if false), and similarly with x_j
// Fields:
//   x: a vector of booleans for one possible valid assignment, or empty if
//     no valid assignment exists
// In practice, has a large constant
// Time Complexity:
//   constructor: O(N + M) for M equations
// Memory Complexity: O(N + M)
// Tested:
//   https://judge.yosupo.jp/problem/two_sat
//   https://codeforces.com/contest/1215/problem/F
struct TwoSat {
  SCC scc; vector<bool> x;
  vector<vector<int>> init(
      int N, const vector<tuple<bool, int, bool, int>> &disjunctions) {
    vector<vector<int>> adj(N * 2); for (auto &&d : disjunctions) {
      int i = get<1>(d) * 2 + get<0>(d), j = get<3>(d) * 2 + get<2>(d);
      adj[i ^ 1].push_back(j); adj[j ^ 1].push_back(i);
    }
    return adj;
  }
  TwoSat(int N, const vector<tuple<bool, int, bool, int>> &disjunctions)
      : scc(init(N, disjunctions)) {
    for (int i = 0; i < N; i++) if (scc.id[i * 2] == scc.id[i * 2 + 1]) return;
    x.assign(N, false);
    for (int i = 0; i < N; i++) x[i] = scc.id[i * 2] > scc.id[i * 2 + 1];
  }
};
