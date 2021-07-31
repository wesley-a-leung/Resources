#pragma once
#include <bits/stdc++.h>
using namespace std;

// 2D Sparse Table supporting associative and idempotent range queries on
//   a static 2D array
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   Op: a struct with the operation (can also be of type
//       std::function<T(T, T)>); in practice, custom struct is faster than
///      std::function
//     Required Functions:
//       operator (l, r): combines the values l and r, must be
//         associative and idempotent
// Constructor Arguments:
//   A: a 2D vector of elements of type T
//   op: an instance of the Op struct
// Functions:
//   query(u, d, l, r): returns the aggregate value of the elements in
//     the range [u, d] in the first dimension and [l, r] in
//     the second dimension
// In practice, the constructor has a small constant,
//   query has a moderate constant, but still faster than segment trees
// Time Complexity:
//   constructor: O(NM log N log M)
//   query: O(1)
// Memory Complexity: O(NM log N log M)
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/2drmq
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=972
template <class T, class Op> struct SparseTable2D {
  int N, M, lgN, lgM; vector<vector<vector<vector<T>>>> ST; Op op;
  SparseTable2D(const vector<vector<T>> &A, Op op = Op())
      : N(A.size()), M(N == 0 ? 0 : A[0].size()),
        lgN(N == 0 ? 0 : __lg(N) + 1), lgM(M == 0 ? 0 : __lg(M) + 1),
        ST(lgN, vector<vector<vector<T>>>(lgM, A)), op(op) {
    for (int ic = 0; ic < lgM - 1; ic++) for (int jr = 0; jr < N; jr++)
      for (int jc = 0; jc < M; jc++) ST[0][ic + 1][jr][jc] = op(
          ST[0][ic][jr][jc], ST[0][ic][jr][min(jc + (1 << ic), M - 1)]);
    for (int ir = 0; ir < lgN - 1; ir++) for (int ic = 0; ic < lgM; ic++)
      for (int jr = 0; jr < N; jr++) for (int jc = 0; jc < M; jc++)
        ST[ir + 1][ic][jr][jc] = op(
            ST[ir][ic][jr][jc], ST[ir][ic][min(jr + (1 << ir), N - 1)][jc]);
  }
  T query(int u, int d, int l, int r) {
    int ir = __lg(d - u + 1), ic = __lg(r - l + 1);
    return op(op(ST[ir][ic][u][l],
                 ST[ir][ic][d - (1 << ir) + 1][r - (1 << ic) + 1]),
              op(ST[ir][ic][d - (1 << ir) + 1][l],
                 ST[ir][ic][u][r - (1 << ic) + 1]));
  }
};
