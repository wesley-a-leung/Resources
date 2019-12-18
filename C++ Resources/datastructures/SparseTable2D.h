#pragma once
#include <bits/stdc++.h>
using namespace std;

// 2 Dimensional Sparse Table for generic operations
// F must be a binary operator that is associative and idempotent
// Time Complexity:
//   init: O(NM log N log M)
//   query: O(1)
// Memory Complexity: O(NM log N log M)
template <class T, class F> struct SparseTable2D {
    int N, M, lgN, lgM; vector<vector<vector<vector<T>>>> ST; F op;
    SparseTable2D(const vector<vector<T>> &v, F op) : N(int(v.size())), M(int(v.back().size())),
            lgN(32 - __builtin_clz(N)), lgM(32 - __builtin_clz(M)),
            ST(lgN, vector<vector<vector<T>>>(lgM, vector<vector<T>>(N, vector<T>(M, v[0][0])))), op(op) {
        for (int jr = 0; jr < N; jr++) for (int jc = 0; jc < M; jc++) ST[0][0][jr][jc] = v[jr][jc];
        for (int ic = 0; ic < lgM - 1; ic++) for (int jr = 0; jr < N; jr++) for (int jc = 0; jc < M; jc++)
            ST[0][ic + 1][jr][jc] = op(ST[0][ic][jr][jc], ST[0][ic][jr][min(jc + (1 << ic), M - 1)]);
        for (int ir = 0; ir < lgN - 1; ir++) for (int ic = 0; ic < lgM; ic++) for (int jr = 0; jr < N; jr++) for (int jc = 0; jc < M; jc++)
            ST[ir + 1][ic][jr][jc] = op(ST[ir][ic][jr][jc], ST[ir][ic][min(jr + (1 << ir), N - 1)][jc]);
    }
    SparseTable2D(const vector<vector<T>> &v): SparseTable2D(v, F()) {}
    // 0-indexed, inclusive
    T query(int u, int d, int l, int r) {
        int ir = 31 - __builtin_clz(d - u + 1), ic = 31 - __builtin_clz(r - l + 1);
        return op(op(ST[ir][ic][u][l], ST[ir][ic][d - (1 << ir) + 1][r - (1 << ic) + 1]),
            op(ST[ir][ic][d - (1 << ir) + 1][l], ST[ir][ic][u][r - (1 << ic) + 1]));
    }
};

template <class T, class F> auto makeSparseTable2D(const vector<vector<T>> &v, F op) {
    return SparseTable2D<T, F>(v, op);
}
