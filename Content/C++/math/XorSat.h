#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solve the xor boolean satisfiability problem for a boolean matrix A of
//   size M x N, and vectors b of size M, and vector x of size N, so that
//   b_i = xor(A[i][j] & x[j]) for 0 <= j < N; equivalent to solving the
//   equation Ax = b mod 2 for a matrix A, and vectors b, x where all values
//   are 0 or 1
// Template Arguments:
//   MAXN: the maximmum value of N
// Constructor Arguments:
//   A: a M x N boolean matrix 
//   b: a boolean vector, with dimension M
// Fields:
//   NO_SOLUTION: static const int representing no solution
//   ONE_SOLUTION: static const int representing one solution
//   INF_SOLUTIONS: static const int representing infinite solution
//   M: the number of equations
//   N: the number of variables
//   solutions: the number of solution to the equation, equal to one of
//     NO_SOLUTION, ONE_SOLUTION, or INF_SOLUTIONS
//   x: the solution vector with dimension N, can be any solution if there are
//     infinite solutions, undefined if no solutions
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(min(M, N) M MAXN / 64)
// Memory Complexity: O(M MAXN / 64)
// Tested:
//   https://dmoj.ca/problem/tle16c1p6
template <const int MAXN> struct XorSat {
  static const int NO_SOLUTION = 0, ONE_SOLUTION = 1, INF_SOLUTIONS = 2;
  int M, N, solutions; bitset<MAXN> x;
  XorSat(vector<bitset<MAXN>> A, vector<bool> b)
      : M(A.size()), N(M == 0 ? 0 : A[0].size()), solutions(ONE_SOLUTION) {
    vector<int> ind(N, -1); for (int r = 0, c = 0; c < N && r < M; c++) {
      int mx = r; for (int i = r + 1; i < M; i++)
        if (A[i][c]) { mx = i; break; }
      if (!A[mx][c]) continue;
      if (r != mx) { swap(A[r], A[mx]); swap(b[r], b[mx]); }
      ind[c] = r; for (int i = 0; i < M; i++)
        if (i != r && A[i][c]) { b[i] = b[i] ^ b[r]; A[i] ^= A[r]; }
      r++;
    }
    for (int i = 0; i < N; i++) if (ind[i] != -1) x[i] = b[ind[i]];
    for (int i = 0; i < M; i++) {
      bool val = false; for (int j = 0; j < N; j++) val ^= x[j] & A[i][j];
      if (val != b[i]) { solutions = NO_SOLUTION; x.reset(); return; }
    }
    for (int i = 0; i < N; i++) if (ind[i] == -1) solutions = INF_SOLUTIONS;
  }
};
