#pragma once
#include <bits/stdc++.h>
using namespace std;

// Disjoint Sparse Table supporting associative range queries on a static array
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   Op: a struct with the operation (can also be of type
//       std::function<T(T, T)>); in practice, custom struct is faster than
///      std::function
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative
// Constructor Arguments:
//   A: a vector of type T
//   op: an instance of the Op struct
// Functions:
//   query(l, r): returns the aggregate value of the elements in
//     the range [l, r] 
// In practice, the constructor has a small constant,
//   query has a moderate constant, but still faster than segment trees,
//   slightly faster than Fischer Heun Structure, and performs similarly to
//   sparse tables
// Time Complexity:
//   constructor: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
// Tested:
//   Fuzz and Stress Tested
//   https://www.spoj.com/problems/GSS1/
//   https://www.spoj.com/problems/GSS5/
//   https://judge.yosupo.jp/problem/staticrmq
template <class T, class Op> struct DisjointSparseTable {
  int N; vector<vector<T>> ST; Op op;
  DisjointSparseTable(vector<T> A, Op op = Op())
      : N(A.size()), ST(N == 0 ? 0 : __lg(N * 2 - 1) + 1, move(A)), op(op) {
    for (int i = 1; i < int(ST.size()); i++) {
      ST[i] = ST[0]; for (int j = 0, len = 1 << i; j < N; j += len) {
        int k = min(j + len / 2, N) - 1; T agg = ST[i][k] = ST[0][k];
        for (k--; k >= j; k--) agg = ST[i][k] = op(ST[0][k], agg);
        k = j + len / 2; if (k < N) agg = ST[i][k] = ST[0][k];
        for (k++; k < min(j + len, N); k++) agg = ST[i][k] = op(agg, ST[0][k]);
      }
    }
  }
  T query(int l, int r) {
    if (l == r) return ST[0][l];
    int i = __lg(l ^ r) + 1; return op(ST[i][l], ST[i][r]);
  }
};
