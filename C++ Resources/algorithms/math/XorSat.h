#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the boolean satisfiability problem for so the rows of A when xored evaluate to b
// If A[i][j] = 1, then the value xj is xored in equation i
// If A[i][j] = 0, then the value !xj is xored in equation i
// Time Complexity: O(N^3)
// Memory Complexity: O(N^2)
template <const int MAXN> struct GaussianElimination {
    bool A[MAXN][MAXN], b[MAXN], x[MAXN];
    bool solve(int N) {
        for (int p = 0; p < N; p++) {
            int m = p;
            for (int i = 0; i < p; i++) {
                int j = i;
                while (j < N && !A[i][j]) j++;
                if (j == p) m = i;
            }
            for (int i = p + 1; i < N; i++) if (A[i][p]) m = i;
            if (!A[m][p]) continue;
            swap(A[p], A[m]); swap(b[p], b[m]);
            for (int i = 0; i < N; i++) if (p != i && A[i][p]) {
                b[i] ^= b[p];
                for (int j = p; j < N; j++) A[i][j] ^= A[p][j];
            }
        }
        copy(b, b + N, x);
        for (int i = N - 1; i >= 0; i--) {
            if (!A[i][i] && x[i]) return false; // no solution
            if (x[i]) for (int j = i - 1; j >= 0; j--) { x[j] ^= A[j][i]; A[j][i] = false; }
        }
        return true;
    }
};
