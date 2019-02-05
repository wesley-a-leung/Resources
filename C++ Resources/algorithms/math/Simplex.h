#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solve a linear programming problem in canonical form:
// max c^T x
// subject to Ax <= b and x >= 0
// where A is an M x N Matrix; b is a Vector with dimension M; c, x are Vectors with dimension N
// Time Complexity: exponential worst case, polynomial in the average case ~O(N * M * (N + M))
// Memory Complexity: O(M * (N + M))
template <const int MAXM, const int MAXN, class F> struct Simplex {
    F EPS, A[MAXM][MAXN], b[MAXM], c[MAXN], x[MAXN], T[MAXM][MAXN + MAXM]; Simplex(F EPS) : EPS(EPS) {}
    F solve(int M, int N) {
        for (int i = 0; i <= M; i++) fill(T[i], T[i] + N + M + 1, 0);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) T[i][j] = A[i][j];
            T[i][N + i] = 1;
            T[i][N + M] = b[i];
        }
        for (int j = 0; j < N; j++) T[M][j] = -c[j];
        while (true) {
            int p = 0, q = 0;
            for (int i = 0; i < M; i++) if (T[i][N + M] <= T[p][N + M]) p = i;
            for (int j = 0; j < N + M; j++) if (T[M][j] <= T[M][q]) q = j;
            F t = min(T[p][N + M], T[M][q]);
            if (t >= -EPS) break; // optimal
            if (t < T[p][N + M]) {
                for (int i = 0; i < M; i++) if (T[i][q] >= EPS && T[i][q] * (T[p][N + M] - t) >= T[p][q] * (T[i][N + M] - t)) p = i;
                if (T[p][q] <= EPS) return numeric_limits<F>::infinity(); // unbounded
            } else {
                for (int j = 0; j <= N + M; j++) T[p][j] *= -1;
                for (int j = 0; j < N + M; j++) if (T[p][j] >= EPS && T[p][j] * (T[M][q] - t) >= T[p][q] * (T[M][j] - t)) q = j;
                if (T[p][q] <= EPS) return -numeric_limits<F>::infinity(); // infeasible
            }
            for (int j = 0; j <= N + M; j++) if (j != q) T[p][j] /= T[p][q];
            T[p][q] = 1;
            for (int i = 0; i <= M; i++) if (i != p) {
                F alpha = T[i][q];
                if (abs(alpha) > EPS) for (int j = 0; j <= N + M; j++) T[i][j] -= T[p][j] * alpha;
            }
        }
        for (int i = 0; i < M; i++) for (int j = 0; j < N; j++) if (T[i][j] >= 0.5) x[j] = T[i][N + M];
        return T[M][N + M];
    }
};
