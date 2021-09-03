#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sparse Table supporting associative and idempotent range queries on
//   a static array
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
//   A: a vector of type T
//   op: an instance of the Op struct
// Functions:
//   query(l, r): returns the aggregate value of the elements in
//     the range [l, r]
// In practice, the constructor has a small constant,
//   query has a moderate constant, but still faster than segment trees,
//   slightly faster than Fischer Heun Structure, and performs similarly to
//   disjoint sparse tables
// Time Complexity:
//   constructor: O(N log N)
//   query: O(1)
// Memory Complexity: O(N log N)
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/ncco3d2p1
//   https://www.spoj.com/problems/RMQSQ/
//   https://judge.yosupo.jp/problem/staticrmq
template <class T, class Op> struct SparseTable {
  int N; vector<vector<T>> ST; Op op;
  SparseTable(vector<T> A, Op op = Op())
      : N(A.size()), ST(N == 0 ? 0 : __lg(N) + 1, move(A)), op(op) {
    for (int i = 0; i < int(ST.size()) - 1; i++) {
      ST[i + 1] = ST[0]; for (int j = 0; j < N; j++)
        ST[i + 1][j] = op(ST[i][j], ST[i][min(j + (1 << i), N - 1)]);
    }
  }
  T query(int l, int r) {
    int i = __lg(r - l + 1); return op(ST[i][l], ST[i][r - (1 << i) + 1]);
  }
};
