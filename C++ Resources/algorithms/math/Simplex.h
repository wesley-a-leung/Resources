#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solve a linear programming problem in canonical form:
// max c^T x
// subject to Ax <= b and x >= 0
// where A is an M x N Matrix; b is a Vector with dimension M; c, x are Vectors with dimension N
// Time Complexity: exponential worst case, polynomial in the average case ~O((M + N)^2.5)
// Memory Complexity: O(MN)
template <const int MAXM, const int MAXN, class F> struct Simplex {
    F EPS, A[MAXM][MAXN], b[MAXM], c[MAXN], x[MAXN], T[MAXM][MAXN]; int ind[MAXM + MAXN]; Simplex(F EPS) : EPS(EPS) {}
    F solve(int M, int N) {
        N++; iota(ind, ind + M + N, 0); int L = M;
        for (int i = 0; i < M + 2; i++) fill(T[i], T[i] + N + 1, 0);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N - 1; j++) T[i][j] = -A[i][j];
            T[i][N - 1] = 1; T[i][N] = b[i];
            if (T[L][N] > T[i][N]) L = i;
        }
        for (int j = 0; j < N - 1; j++) T[M][j] = c[j];
        T[M + 1][N - 1] = -1;
        for (int E = N - 1; ;) {
            if (L < M) {
                swap(ind[E], ind[L + N]); T[L][E] = 1.0 / T[L][E];
                for (int j = 0; j <= N; j++) if (j != E) T[L][j] = T[L][j] * -T[L][E];
                for (int i = 0; i <= M + 1; i++) if (i != L) {
                    for (int j = 0; j <= N; j++) if (j != E) T[i][j] = T[i][j] + T[L][j] * T[i][E];
                    T[i][E] = T[i][E] * T[L][E];
                }
            }
            E = -1;
            for (int j = 0; j < N; j++)
                if ((E < 0 || ind[E] > ind[j]) && (T[M + 1][j] > 0 || (abs(T[M + 1][j]) <= EPS && T[M][j] > 0))) E = j;
            if (E < 0) break;
            L = -1;
            for (int i = 0; i < M; i++) if (T[i][E] < 0) {
                double d;
                if (L < 0 || (d = T[L][N] / T[L][E] - T[i][N] / T[i][E]) < 0 || (abs(d) <= EPS && ind[L + N] > ind[i + N])) L = i;
            }
            if (L < 0) return numeric_limits<F>::infinity(); // unbounded
        }
        if (T[M + 1][N] < -EPS) throw new runtime_error("No feasible solution");
        fill(x, x + N - 1, 0);
        for (int i = 0; i < M; i++) if (ind[N + i] < N - 1) x[ind[N + i]] = T[i][N];
        return T[M][N];
    }
};
