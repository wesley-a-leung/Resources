#pragma once
#include <bits/stdc++.h>
using namespace std;

// 2 Dimensional Sparse Table for generic operations
// F must be a binary operator that is associative and idempotent
// Time Complexity:
//   init: O(NM log N log M)
//   query: O(1)
// Memory Complexity: O(NM log N log M)
template <const int MAXN, const int MAXM, class T, class F> struct SparseTable2D {
    T ST[32 - __builtin_clz(MAXN)][32 - __builtin_clz(MAXM)][MAXN][MAXM]; F op;
    void init(const vector<vector<T>> &v, F op = F()) {
        this->op = op; int N = int(v.size()), M = int(v.back().size()), lgN = 32 - __builtin_clz(N), lgM = 32 - __builtin_clz(M);
        for (int jr = 0; jr < N; jr++) for (int jc = 0; jc < M; jc++) ST[0][0][jr][jc] = v[jr][jc];
        for (int ic = 0; ic < lgM - 1; ic++) for (int jr = 0; jr < N; jr++) for (int jc = 0; jc < M; jc++)
            ST[0][ic + 1][jr][jc] = op(ST[0][ic][jr][jc], ST[0][ic][jr][min(jc + (1 << ic), M - 1)]);
        for (int ir = 0; ir < lgN - 1; ir++) for (int ic = 0; ic < lgM; ic++) for (int jr = 0; jr < N; jr++) for (int jc = 0; jc < M; jc++)
            ST[ir + 1][ic][jr][jc] = op(ST[ir][ic][jr][jc], ST[ir][ic][min(jr + (1 << ir), N - 1)][jc]);
    }
    // 0-indexed, inclusive
    T query(int u, int d, int l, int r) {
        int ir = 31 - __builtin_clz(d - u + 1), ic = 31 - __builtin_clz(r - l + 1);
        return op(op(ST[ir][ic][u][l], ST[ir][ic][d - (1 << ir) + 1][r - (1 << ic) + 1]),
            op(ST[ir][ic][d - (1 << ir) + 1][l], ST[ir][ic][u][r - (1 << ic) + 1]));
    }
};
