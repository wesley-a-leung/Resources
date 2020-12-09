#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solve the equation Ax = b for a matrix A of size M x N, and vectors b of
//   size M, and vector x of size N
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   A: a M x N matrix 
//   b: a vector, with dimension M
//   EPS: a value for epsilon
// Fields:
//   NO_SOLUTION: static const int representing no solution
//   ONE_SOLUTION: static const int representing one solution
//   INF_SOLUTIONS: static const int representing infinite solution
//   M: the number of equations
//   N: the number of variables
//   solutions: the number of solution to the equation, equal to one of
//     NO_SOLUTION, ONE_SOLUTION, or INF_SOLUTIONS
//   EPS: the value for epsilon
//   x: the solution vector with dimension N, can be any solution if there are
//     infinite solutions, undefined if no solutions
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(min(M, N) MN)
// Memory Complexity: O(MN)
// Tested:
//   https://open.kattis.com/problems/equationsolver
template <class T> struct GaussianElimination {
  static const int NO_SOLUTION = 0, ONE_SOLUTION = 1, INF_SOLUTIONS = 2;
  T abs(T a) { return a >= 0 ? a : -a; }
  int M, N, solutions; T EPS; vector<T> x;
  GaussianElimination(vector<vector<T>> A, vector<T> b, T EPS = T(1e-9))
      : M(A.size()), N(M == 0 ? 0 : A[0].size()), solutions(ONE_SOLUTION),
        EPS(EPS), x(N, T()) {
    vector<int> ind(N, -1); for (int r = 0, c = 0; c < N && r < M; c++) {
      int mx = r; for (int i = r + 1; i < M; i++)
        if (abs(A[i][c]) > abs(A[mx][c])) mx = i;
      if (abs(A[mx][c]) <= EPS) continue;
      if (r != mx) { A[r].swap(A[mx]); swap(b[r], b[mx]); }
      ind[c] = r; T inv = T(1) / A[r][c];
      for (int i = 0; i < M; i++) if (i != r) {
        T alpha = A[i][c] * inv; b[i] -= b[r] * alpha;
        for (int j = c; j < N; j++) A[i][j] -= A[r][j] * alpha;
      }
      r++;
    }
    for (int i = 0; i < N; i++) if (ind[i] != -1)
      x[i] = b[ind[i]] / A[ind[i]][i];
    for (int i = 0; i < M; i++) {
      T sm = T(); for (int j = 0; j < N; j++) sm += x[j] * A[i][j];
      if (abs(sm - b[i]) > EPS) {
        solutions = NO_SOLUTION; x.clear(); return;
      }
    }
    for (int i = 0; i < N; i++) if (ind[i] == -1) solutions = INF_SOLUTIONS;
  }
};
