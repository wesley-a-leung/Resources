#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solve a linear programming problem in canonical form:
// max c^T x
// subject to Ax <= b and x >= 0
// where A is an M x N Matrix; b is a Vector with dimension M; c, x are Vectors with dimension N
// Time Complexity: exponential worst case, polynomial in the average case ~O((M + N) * M * N)
// Memory Complexity: O(M * N)
template <const int MAXM, const int MAXN, class F> struct Simplex {
    F INF, EPS, A[MAXM][MAXN], b[MAXM], c[MAXN], x[MAXN], T[MAXM][MAXN + MAXM]; int IN[MAXM], OUT[MAXN + MAXM];
    Simplex(F INF, F EPS) : INF(INF), EPS(EPS) {}
    bool cmp(F a, int b, F c, int d) { return abs(a - c) > EPS ? a < c : b < d; }
    F solve(int M, int N) {
        for (int i = 0; i <= M; i++) fill(T[i], T[i] + N + M + 1, 0);
        fill(IN, IN + M + 1, -1); fill(OUT, OUT + N + M + 1, N);
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) T[i][j] = A[i][j];
            T[i][N + i] = 1;
            T[i][N + M] = b[i];
            IN[i] = -(N + M + i);
        }
        for (int j = 0; j < N; j++) T[M][OUT[j] = j] = -c[j];
        for (int j = N; j < N + M; j++) OUT[j] = j;
        while (true) {
            int p = 0, q = 0;
            for (int i = 0; i < M; i++) if (cmp(T[i][N + M], IN[i], T[p][N + M], IN[p])) p = i;
            for (int j = 0; j < N + M; j++) if (cmp(T[M][j], OUT[j], T[M][q], OUT[q])) q = j;
            F t = min(T[p][N + M], T[M][q]);
            if (t >= -EPS) break; // optimal
            if (t < T[p][N + M]) {
                for (int i = 0; i < M; i++) if (T[i][q] > EPS && cmp(T[p][q] * (T[i][N + M] - t), IN[i], T[i][q] * (T[p][N + M] - t), IN[p])) p = i;
                if (T[p][q] <= EPS) return INF; // unbounded
            } else {
                for (int j = 0; j <= N + M; j++) T[p][j] *= -1;
                for (int j = 0; j < N + M; j++) if (T[p][j] > EPS && cmp(T[p][q] * (T[M][j] - t), OUT[j], T[p][j] * (T[M][q] - t), OUT[q])) q = j;
                if (T[p][q] <= EPS) return -INF; // infeasible
                for (int j = 0; j <= N + M; j++) T[p][j] *= -1;
            }
            for (int j = 0; j <= N + M; j++) if (j != q) T[p][j] /= T[p][q];
            for (int i = 0; i <= M; i++) if (i != p) {
                F alpha = T[i][q];
                if (abs(alpha) > EPS) {
                    for (int j = 0; j <= N + M; j++) if (abs(T[p][j]) > EPS) T[i][j] -= T[p][j] * alpha;
                    T[i][q] = -alpha / T[p][q];
                } else T[i][q] = 0;
            }
            T[p][q] = 1 / T[p][q]; swap(IN[p], OUT[q]);
        }
        fill(x, x + N, 0); F checkVal = 0;
        for (int i = 0; i < M; i++) if (0 <= IN[i] && IN[i] < N) x[IN[i]] = T[i][N + M];
        for (int i = 0; i < N; i++) checkVal += x[i] * c[i];
        assert(abs(checkVal - T[M][N + M]) <= sqrt(EPS)); return T[M][N + M];
    }
};
