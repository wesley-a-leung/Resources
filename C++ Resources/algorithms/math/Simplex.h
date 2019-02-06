#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solve a linear programming problem in canonical form:
// max c^T x
// subject to Ax <= b and x >= 0
// where A is an M x N Matrix; b is a Vector with dimension M; c, x are Vectors with dimension N
// Time Complexity: exponential worst case, polynomial in the average case ~O(N * M * (N + M))
// Memory Complexity: O(M * N)
template <const int MAXM, const int MAXN, class F> struct Simplex {
    int M, N, IN[MAXM], OUT[MAXN]; F INF, EPS, A[MAXM][MAXN], b[MAXM], c[MAXN], x[MAXN], T[MAXM][MAXN];
    Simplex(const F INF, const F EPS) : INF(INF), EPS(EPS) {}
    void pivot(int r, int s) {
        auto &a1 = T[r]; F inv = 1 / a1[s];
        for (int i = 0; i <= M + 1; i++) if (i != r) {
            auto &a2 = T[i]; F inv2 = a2[s] * inv;
            for (int j = 0; j <= N + 1; j++) a2[j] -= a1[j] * inv2;
            a2[s] = a1[s] * inv2;
        }
        for (int j = 0; j <= N + 1; j++) if (j != s) a1[j] *= inv;
        for (int i = 0; i <= M + 1; i++) if (i != r) T[i][s] *= -inv;
        a1[s] = inv; swap(IN[r], OUT[s]);
    }
    bool simplex(int phase) {
        int x = M + phase - 1;
        while (true) {
            int s = -1;
            for (int j = 0; j <= N; j++) if (OUT[j] != -phase && (s == -1 || make_pair(T[x][j], OUT[j]) < make_pair(T[x][s], OUT[s]))) s = j;
            if (T[x][s] >= -EPS) return true;
            int r = -1;
            for (int i = 0; i < M; i++)
                if (T[i][s] > EPS && (r == -1 || make_pair(T[i][N + 1] / T[i][s], IN[i]) < make_pair(T[r][N + 1] / T[r][s], IN[r]))) r = i;
            if (r == -1) return false;
            pivot(r, s);
        }
    }
    F solve(int m, int n) {
        M = m; N = n;
        for (int i = 0; i <= M + 1; i++) fill(T[i], T[i] + N + 1, 0);
        for (int i = 0; i < M; i++) for (int j = 0; j < N; j++) T[i][j] = A[i][j];
        for (int i = 0; i < M; i++) { IN[i] = N + i; T[i][N] = -1; T[i][N + 1] = b[i]; }
        for (int j = 0; j < N; j++) { OUT[j] = j; T[M][j] = -c[j]; }
        OUT[N] = -1; T[M + 1][N] = 1;
        int r = 0;
        for (int i = 1; i < M; i++) if (T[i][N + 1] < T[r][N + 1]) r = i;
        if (T[r][N + 1] < -EPS) {
            pivot(r, N);
            if (!simplex(2) || T[M + 1][N + 1] < -EPS) return -INF; // infeasible
            for (int i = 0; i < M; i++) if (IN[i] == -1) {
                int s = 0;
                for (int j = 1; j <= N; j++) if (s == -1 || make_pair(T[i][j], OUT[j]) < make_pair(T[i][s], OUT[s])) s = j;
                pivot(i, s);
            }
        }
        bool unbounded = !simplex(1); fill(x, x + N, 0);
        for (int i = 0; i < M; i++) if (IN[i] < N) x[IN[i]] = T[i][N + 1];
        return unbounded ? INF : T[M][N + 1];
    }
};
