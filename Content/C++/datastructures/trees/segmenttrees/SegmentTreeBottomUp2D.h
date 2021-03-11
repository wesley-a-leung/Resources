#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bottom up 2D segment tree supporting point updates and range queries
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//     Sample Struct: supporting point increments and range max queries
//       struct C {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return numeric_limits<int>::min(); }
//         static Data merge(const Data &l, const Data &r) {
//           return max(l, r);
//         }
//         static Data applyLazy(const Data &l, const Lazy &r) {
//           return l + r;
//         }
//       };
// Constructor Arguments:
//   N: the size of the first dimension of the array
//   M: the size of the second dimension of the array
//   A: a 2D vector of elements of type Data
//   vdef: the default value to fill the array with
// Functions:
//   update(i, j, v): updates the value A[i][j] with the lazy value v
//   query(u, d, l, r): queries the range [u, d] in the first dimension and
//     [l, r] in the second dimension and returns the aggregate value
// In practice, has a small constant, not quite as fast as fenwick trees
// Time Complexity:
//   constructor: O(NM)
//   update, query: O(log N log M)
// Memory Complexity: O(NM)
// Tested:
//   Fuzz and Stress Tested
template <class C> struct SegmentTreeBottomUp2D {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  int N, M; vector<vector<Data>> TR;
  void build() {
    for (int i = 0; i < N; i++) for (int j = M - 1; j > 0; j--)
      TR[N + i][j] = C::merge(TR[N + i][j * 2], TR[N + i][j * 2 + 1]);
    for (int i = N - 1; i > 0; i--) {
      for (int j = 0; j < M; j++)
        TR[i][M + j] = C::merge(TR[i * 2][M + j], TR[i * 2 + 1][M + j]);
      for (int j = M - 1; j > 0; j--)
        TR[i][j] = C::merge(TR[i][j * 2], TR[i][j * 2 + 1]);
    }
  }
  SegmentTreeBottomUp2D(const vector<vector<Data>> &A)
      : N(A.size()), M(N == 0 ? 0 : A[0].size()),
        TR(N * 2, vector<Data>(M * 2, C::qdef())) {
    for (int i = 0; i < N; i++)
      copy(A[i].begin(), A[i].end(), TR[N + i].begin() + M);
    build();
  }
  SegmentTreeBottomUp2D(int N, int M, const Data &vdef)
      : N(N), M(M), TR(N * 2, vector<Data>(M * 2, C::qdef())) {
    for (int i = 0; i < N; i++)
      fill(TR[N + i].begin() + M, TR[N + i].end(), vdef);
    build();
  }
  void update(int i, int j, const Lazy &v) {
    for (int ii = N + i; ii > 0; ii /= 2) {
      if (ii == N + i) TR[ii][M + j] = C::applyLazy(TR[N + i][M + j], v);
      else TR[ii][M + j] = C::merge(TR[ii * 2][M + j], TR[ii * 2 + 1][M + j]);
      for (int jj = M + j; jj /= 2;)
        TR[ii][jj] = C::merge(TR[ii][jj * 2], TR[ii][jj * 2 + 1]);
    }
  }
  Data query(int u, int d, int l, int r) {
    auto query = [&] (int i, int l, int r) {
      Data ql = C::qdef(), qr = C::qdef();
      for (l += M, r += M; l <= r; l /= 2, r /= 2) {
        if (l % 2) ql = C::merge(ql, TR[i][l++]);
        if (!(r % 2)) qr = C::merge(TR[i][r--], qr);
      }
      return C::merge(ql, qr);
    };
    Data qu = C::qdef(), qd = C::qdef();
    for (u += N, d += N; u <= d; u /= 2, d /= 2) {
      if (u % 2) qu = C::merge(qu, query(u++, l, r));
      if (!(d % 2)) qd = C::merge(query(d--, l, r), qd);
    }
    return C::merge(qu, qd);
  }
};
