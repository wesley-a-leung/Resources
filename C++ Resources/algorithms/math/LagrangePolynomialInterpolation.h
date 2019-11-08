#pragma once
#include <bits/stdc++.h>
using namespace std;

// Given a set of N points, (X[i], Y[i]), compute a polynomial P of degree N - 1,
// such that P passes through them. P(x) = A[0] * x^0 + ... + A[N - 1] * x^(N - 1).
// For real numbers, pick X[i] = C * cos(i / (N - 1) * PI) for a large constant C
template <const int MAXN, class T> struct LagrangePolynomialInterpolation {
    T X[MAXN], Y[MAXN], A[MAXN], temp[MAXN];
    void solve(int N) {
        for (int k = 0; k < N - 1; k++) for (int i = k + 1; i < N; i++) Y[i] = (Y[i] - Y[k]) / (X[i] - X[k]);
        T last = 0; temp[0] = 1; fill(temp + 1, temp + N, 0); fill(A, A + N, 0);
        for (int k = 0; k < N; k++) for (int i = 0; i < N; i++) {
            A[i] += Y[k] * temp[i]; swap(last, temp[i]); temp[i] -= X[k] * last;
        }
    }
};
