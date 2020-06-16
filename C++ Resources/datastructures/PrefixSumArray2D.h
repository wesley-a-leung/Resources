#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class C> void partial_sum_2d(const C &A, int N, int M, C &pre) {
    for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) {
        pre[i][j] = A[i][j];
        if (i > 0) pre[i][j] += pre[i - 1][j];
        if (j > 0) pre[i][j] += pre[i][j - 1];
        if (i > 0 && j > 0) pre[i][j] -= pre[i - 1][j - 1];
    }
}

template <class C> void adjacent_difference_2d(const C &diff, int N, int M, C &A) {
    for (int i = N - 1; i >= 0; i--) for (int j = M - 1; j >= 0; j--) {
        A[i][j] = diff[i][j];
        if (i > 0) A[i][j] -= diff[i - 1][j];
        if (j > 0) A[i][j] -= diff[i][j - 1];
        if (i > 0 && j > 0) A[i][j] += diff[i - 1][j - 1];
    }
}

template <class C> auto rsq(const C &pre, int u, int d, int l, int r) -> typename decay<decltype(pre[d][r])>::type {
    typename decay<decltype(pre[d][r])>::type val = pre[d][r];
    if (u > 0) val -= pre[u - 1][r];
    if (l > 0) val -= pre[d][l - 1];
    if (u > 0 && l > 0) val += pre[u - 1][l - 1];
    return val;
}

template <class C, class T> void add(C &diff, int N, int M, int u, int d, int l, int r, T val) {
    diff[u][l] += val;
    if (d + 1 < N) diff[d + 1][l] -= val;
    if (r + 1 < M) diff[u][r + 1] -= val;
    if (d + 1 < N && r + 1 < M) diff[d + 1][r + 1] += val;
}
