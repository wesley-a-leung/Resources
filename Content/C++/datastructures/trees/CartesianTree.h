#pragma once
#include <bits/stdc++.h>
using namespace std;

// Builds a cartesian tree where all vertices j in vertex i's subtree have
//   !cmp(A[i], A[j]) and (cmp(A[j], A[i]) or j < i)
// Indices are 0-indexed
// Template Arguments:
//   T: the type of each element in the array
//   Cmp: the comparator to compare two values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   A: a vector of type T
//   cmp: an instance of the Cmp struct
// Return Value: a vector of integers representing the parent of each vertex
//   in the cartesian tree, or -1 if that vertex is the root
// In practice, has a moderate constant
// Time Complexity: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/cartesian_tree
template <class T, class Cmp = less<T>>
vector<int> cartesianTree(const vector<T> &A, Cmp cmp = Cmp()) {
  int N = A.size(), top = 0; vector<int> stk(N), P(N, -1);
  for (int i = 0; i < N; stk[top++] = i++) {
    int last = -1; while (top > 0 && cmp(A[stk[top - 1]], A[i])) {
      if (last != -1) P[last] = stk[top - 1];
      last = stk[--top];
    }
    if (last != -1) P[last] = i;
    if (top > 0) P[i] = stk[top - 1];
  }
  return P;
}
