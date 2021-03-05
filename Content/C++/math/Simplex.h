#pragma once
#include <bits/stdc++.h>
using namespace std;

// Two Phase Simplex to solve a linear programming problem with N variables
//   and M equations in canonical form:
//     max c^T x
//     subject to Ax <= b and x >= 0
//     where A is an M x N matrix; b is a vector with dimension M;
//     c, x are vectors with dimension N
// Template Arguments:
//   F: a floating point type
// Constructor Arguments:
//   A: a M x N coefficient matrix 
//   b: a constraint vector, with dimension M
//   c: a cost vector, with dimension N
//   INF: a value for infinity
//   EPS: a value for epsilon
// Fields:
//   M: the number of equations
//   N: the number of variables
//   INF: the value for infinity
//   EPS: the value for epsilon
//   val: the value of the maximum cost of the objective function c^T x,
//     INF if unbounded, -INF if infeasible
//   x: a solution vector of dimension N producing the maximum cost, can be
//     any solution if unbounded, empty if infeasible
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(2^N), worst case, much faster in practice
// Memory Complexity: O(MN)
// Tested:
//   https://ecna19.kattis.com/problems/cheeseifyouplease
//   https://open.kattis.com/problems/goatropes
//   https://codeforces.com/contest/375/problem/E
//   https://codeforces.com/contest/605/problem/C
//   https://codeforces.com/contest/1061/problem/E
//   https://codeforces.com/contest/1288/problem/F
//   https://www.spoj.com/problems/BABY/
template <class F> struct Simplex {
  static_assert(is_floating_point<F>::value,
                "F must be a floating point type");
  int M, N; F INF, EPS, val; vector<int> IN, OUT; vector<F> x;
  vector<vector<F>> T;
  bool cmp(F a, int b, F c, int d) { return abs(a - c) > EPS ? a < c : b < d; }
  void pivot(int r, int s) {
    auto &a1 = T[r]; F inv1 = F(1) / a1[s];
    for (int i = 0; i <= M + 1; i++) if (i != r && abs(T[i][s]) > EPS) {
      auto &a2 = T[i]; F inv2 = a2[s] * inv1;
      for (int j = 0; j <= N + 1; j++) a2[j] -= a1[j] * inv2;
      a2[s] = a1[s] * inv2;
    }
    for (int j = 0; j <= N + 1; j++) if (j != s) a1[j] *= inv1;
    for (int i = 0; i <= M + 1; i++) if (i != r) T[i][s] *= -inv1;
    a1[s] = inv1; swap(IN[r], OUT[s]);
  }
  bool simplex(int phase) {
    int x = M + phase - 1; while (true) {
      int s = -1; for (int j = 0; j <= N; j++)
        if (OUT[j] != -phase
            && (s == -1 || cmp(T[x][j], OUT[j], T[x][s], OUT[s]))) s = j;
      if (T[x][s] >= -EPS) return true;
      int r = -1; for (int i = 0; i < M; i++)
        if (T[i][s] > EPS
            && (r == -1 || cmp(T[i][N + 1] * T[r][s], IN[i],
                               T[r][N + 1] * T[i][s], IN[r]))) r = i;
      if (r == -1) return false;
      pivot(r, s);
    }
  }
  Simplex(const vector<vector<F>> &A, const vector<F> &b, const vector<F> &c,
          F INF = numeric_limits<F>::infinity(), F EPS = F(1e-9))
      : M(b.size()), N(c.size()), INF(INF), EPS(EPS), IN(M), OUT(N + 1),
        T(M + 2, vector<F>(N + 2, F())) {
    for (int i = 0; i < M; i++) {
      copy(A[i].begin(), A[i].end(), T[i].begin());
      IN[i] = N + i; T[i][N] = F(-1); T[i][N + 1] = b[i];
    }
    for (int j = 0; j < N; j++) { OUT[j] = j; T[M][j] = -c[j]; }
    OUT[N] = -1; T[M + 1][N] = F(1); int r = 0;
    for (int i = 1; i < M; i++) if (T[i][N + 1] < T[r][N + 1]) r = i;
    if (T[r][N + 1] < -EPS) {
      pivot(r, N);
      if (!simplex(2) || T[M + 1][N + 1] < -EPS) { val = -INF; return; }
      for (int i = 0; i < M; i++) if (IN[i] == -1) {
        int s = 0; for (int j = 1; j <= N; j++)
          if (s == -1 || cmp(T[i][j], OUT[j], T[i][s], OUT[s])) s = j;
        pivot(i, s);
      }
    }
    bool unbounded = !simplex(1); x.assign(N, F());
    for (int i = 0; i < M; i++) if (IN[i] < N) x[IN[i]] = T[i][N + 1];
    val = unbounded ? INF : T[M][N + 1];
  }
};
