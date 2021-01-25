#pragma once
#include <bits/stdc++.h>
using namespace std;

// Builds a cartesian tree where all vertices j in vertex i's subtree have
//   !cmp(A[i], A[j]) and (cmp(A[j], A[i]) or j < i)
// Indices are 0-indexed
// Template Arguments:
//   It: the type of the iterator
//   Cmp: the comparator to compare two values,
//       convention is same as priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   cmp: an instance of the Cmp struct
// Return Value: a vector of integers representing the parent of each vertex
//   in the cartesian tree, or -1 if that vertex is the root
// In practice, has a moderate constant
// Time Complexity: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/cartesian_tree
template <class It, class Cmp = less<typename iterator_traits<It>::value_type>>
vector<int> cartesianTree(It st, It en, Cmp cmp = Cmp()) {
  int N = en - st, top = 0; vector<int> stk(N), P(N, -1);
  for (int i = 0; i < N; stk[top++] = i++) {
    int last = -1; while (top > 0 && cmp(st[stk[top - 1]], st[i])) {
      if (last != -1) P[last] = stk[top - 1];
      last = stk[--top];
    }
    if (last != -1) P[last] = i;
    if (top > 0) P[i] = stk[top - 1];
  }
  return P;
}
