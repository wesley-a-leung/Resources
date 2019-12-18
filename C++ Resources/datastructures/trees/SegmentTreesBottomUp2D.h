#pragma once
#include <bits/stdc++.h>
using namespace std;

// point updates, range query
template <const int MAXN, const int MAXM> struct SegmentTreeBottomUp2D {
    using Data = int; using Lazy = int; int N, M; Data T[MAXN << 1][MAXM << 1];
    const Data vdef = 0; // default value
    const Data qdef = 0; // query default value
    // operation must be associative (but not necessarily commutative)
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    void init(const vector<vector<Data>> &A) {
        N = int(A.size()); M = int(A.back().size());
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) T[N + i][M + j] = A[i][j];
            for (int j = M - 1; j > 0; j--) T[N + i][j] = merge(T[N + i][j << 1], T[N + i][j << 1 | 1]);
        }
        for (int i = N - 1; i > 0; i--) {
            for (int j = 0; j < M; j++) T[i][M + j] = merge(T[i << 1][M + j], T[i << 1 | 1][M + j]);
            for (int j = M - 1; j > 0; j--) T[i][j] = merge(T[i][j << 1], T[i][j << 1 | 1]);
        }
    }
    void init(int n, int m) {
        N = n; M = m;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) T[N + i][M + j] = vdef;
            for (int j = M - 1; j > 0; j--) T[N + i][j] = merge(T[N + i][j << 1], T[N + i][j << 1 | 1]);
        }
        for (int i = N - 1; i > 0; i--) {
            for (int j = 0; j < M; j++) T[i][M + j] = merge(T[i << 1][M + j], T[i << 1 | 1][M + j]);
            for (int j = M - 1; j > 0; j--) T[i][j] = merge(T[i][j << 1], T[i][j << 1 | 1]);
        }
    }
    void update(int i, int j, const Lazy &v) {
        for (int ii = N + i; ii > 0; ii >>= 1) {
            if (ii == N + i) T[ii][M + j] = applyLazy(T[N + i][M + j], v);
            else T[ii][M + j] = merge(T[ii << 1][M + j], T[ii << 1 | 1][M + j]);
            for (int jj = M + j; jj >>= 1;) T[ii][jj] = merge(T[ii][jj << 1], T[ii][jj << 1 | 1]);
        }
    }
    Data query(int i, int l, int r) {
        Data ql = qdef, qr = qdef;
        for (l += N, r += N; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ql = merge(ql, T[i][l++]);
            if (!(r & 1)) qr = merge(T[i][r--], qr);
        }
        return merge(ql, qr);
    }
    Data query(int u, int d, int l, int r) {
        Data qu = qdef, qd = qdef;
        for (u += N, d += N; u <= d; u >>= 1, d >>= 1) {
            if (u & 1) qu = merge(qu, query(u++, l, r));
            if (!(d & 1)) qd = merge(query(d--, l, r), qd);
        }
        return merge(qu, qd);
    }
};
