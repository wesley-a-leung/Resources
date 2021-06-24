#pragma once
#include <bits/stdc++.h>
#include "../components/StronglyConnectedComponents.h"
using namespace std;

// Solves the two satisfiability problem
// Given an implication graph, determine whether a consistent assignment exists
// Functions for created an implication graph can be seen in ImplicationGraph.h
// Variables are 0-indexed
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
//   x: a vector of booleans for one possible valid assignment, or empty if
//     no valid assignment exists
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N + M) for M equations
// Memory Complexity: O(N + M)
// Tested:
//   https://judge.yosupo.jp/problem/two_sat
//   https://codeforces.com/contest/1215/problem/F
//   https://codeforces.com/contest/780/problem/D
struct TwoSat {
  SCC scc; vector<bool> x;
  template <class ImplicationGraph>
  TwoSat(const ImplicationGraph &G) : scc(G) {
    assert(G.size() % 2 == 0); int N = G.size() / 2;
    for (int i = 0; i < N; i++) if (scc.id[i * 2] == scc.id[i * 2 + 1]) return;
    x.assign(N, false);
    for (int i = 0; i < N; i++) x[i] = scc.id[i * 2] > scc.id[i * 2 + 1];
  }
};
