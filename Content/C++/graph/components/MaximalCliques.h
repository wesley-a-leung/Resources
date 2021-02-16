#pragma once
#include <bits/stdc++.h>
using namespace std;

// Recursive helper function
template <class B, class F>
void maximalCliques(const vector<B> &matrix, F &f, B P, B X, B R) {
  if (!P.any()) {
    if (!X.any()) f(R);
    return;
  }
  auto q = (P | X)._Find_first(); auto cands = P & ~matrix[q];
  for (int i = 0; i < int(matrix.size()); i++) if (cands[i]) {
    R[i] = 1;
    maximalCliques(matrix, f, P & matrix[i], X & matrix[i], R);
    R[i] = P[i] = 0; X[i] = 1;
  }
}

// Runs a callback on the maximal cliques in a graph
// A clique is maximal if any vertex added to it would result in a non-clique
// Vertices are 0-indexed
// Template Arguments:
//   B: a bitset of a fixed size of at least the size of matrix
//   F: the type of the function f
// Function Arguments:
//   matrix: a matrix of bitsets representing the adjacency matrix of the
//     graph, must be symmetric
//   f(s): the function to run a callback on for each clique, where s is a
//     bitset representing which vertices are in the clique
// In practice, has a very small constant
// Time Complexity: O(3^(V / 3)), much faster in practice
// Memory Complexity: O(V^2 / 64)
// Tested:
//   https://open.kattis.com/problems/friends
template <class B, class F>
void maximalCliques(const vector<B> &matrix, F f) {
  maximalCliques(matrix, f, ~B(), B(), B());
}
