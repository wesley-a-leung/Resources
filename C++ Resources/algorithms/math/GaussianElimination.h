#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solve the equation Ax = b for Matrix A, and Vectors b, x
// Time Complexity: O(N^3)
// Memory Complexity: O(N^2)
template <const int MAXN, class T> struct GaussianElimination {
    T EPS, A[MAXN][MAXN], b[MAXN], x[MAXN]; GaussianElimination(T EPS) : EPS(EPS) {}
    void solve(int N) {
        for (int p = 0; p < N; p++) {
            int m = p;
            for (int i = p + 1; i < N; i++) if (abs(A[i][p]) > abs(A[m][p])) m = i;
            swap(A[p], A[m]); swap(b[p], b[m]);
            if (abs(A[p][p]) <= EPS) throw runtime_error("Matrix is singular or nearly singular");
            for (int i = p + 1; i < N; i++) {
                T alpha = A[i][p] / A[p][p]; b[i] -= alpha * b[p];
                for (int j = p; j < N; j++) A[i][j] -= alpha * A[p][j];
            }
        }
        fill(x, x + N, 0);
        for (int i = N - 1; i >= 0; i--) {
            T sum = 0;
            for (int j = i + 1; j < N; j++) sum += A[i][j] * x[j];
            x[i] = (b[i] - sum) / A[i][i];
        }
    }
};
