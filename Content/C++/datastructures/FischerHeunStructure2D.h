#pragma once
#include <bits/stdc++.h>
#include "FischerHeunStructure.h"
using namespace std;

// Fischer Heun Structure in 2 dimensions supporting range maximum queries on
//   a static 2D array
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   Cmp: the comparator to compare two values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
//   mask_t: the type to store a bitmask, should have around log(N) bits
// Constructor Arguments:
//   A: a 2D vector of elements of type T
//   cmp: an instance of the Cmp struct
// Functions:
//   query(u, d, l, r): returns the maximum element (based on the comparator)
//     in the range [u, d] in the first dimension and [l, r] in
//     the second dimension
// In practice, the constructor has a moderate constant and is significantly
//   faster than sparse table's constructor, query has a moderate constant and
//   is slightly slower than sparse table's query
// Time Complexity:
//   constructor: O(N log (N) (M + M / B log (M / B))), where B is
//     the number of bits in mask_t
//   queryInd, query: O(1) assuming bitshift for mask_t is O(1)
// Memory Complexity: O(N log (N) (M + M / B log (M / B))) assuming mask_t is
//   O(1) memory
// Tested:
//   Fuzz and Stress Tested
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=972
template <class T, class Cmp = less<T>, class mask_t = uint32_t>
struct FischerHeunStructure2D {
  int N, M, lgN;
  vector<vector<FischerHeunStructure<T, Cmp, mask_t>>> FHS; Cmp cmp;
  T cmpVal(const T &a, const T &b) { return cmp(a, b) ? b : a; }
  FischerHeunStructure2D(vector<vector<T>> A, Cmp cmp = Cmp())
      : N(A.size()), M(N == 0 ? 0 : A[0].size()),
        lgN(N == 0 ? 0 : __lg(N) + 1), FHS(lgN), cmp(cmp) {
    for (int i = 0; i < lgN; i++) {
      FHS[i].reserve(N); for (int j = 0; j < N; j++)
        FHS[i].emplace_back(A[j].begin(), A[j].end(), cmp);
      if (i < lgN - 1) for (int j = 0; j < N; j++) for (int k = 0; k < M; k++)
        A[j][k] = cmpVal(A[j][k], A[min(j + (1 << i), N - 1)][k]);
    }
  }
  T query(int u, int d, int l, int r) {
    int i = __lg(d - u + 1);
    return cmpVal(FHS[i][u].query(l, r), FHS[i][d - (1 << i) + 1].query(l, r));
  }
};
