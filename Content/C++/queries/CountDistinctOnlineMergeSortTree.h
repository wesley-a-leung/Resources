#pragma once
#include <bits/stdc++.h>
#include "../datastructures/trees/segmenttrees/MergeSortTree.h"
using namespace std;

// Supports online queries for the number of distinct elements in the
//   range [l, r] for a static array A of length N using a merge sort tree
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   A: a vector of type T of the values in the array
// Functions:
//   query(l, r): returns the number of distinct values in the range [l, r]
// In practice, has a small constant, slightly slower than the persistent
//   tree version, but uses less memory
// Time Complexity:
//   constructor: O(N log N)
//   query: O((log N)^2)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.acmicpc.net/problem/14898
template <class T> struct CountDistinctOnlineMergeSortTree {
  MergeSortTree<int> mst;
  MergeSortTree<int> init(const vector<T> &A) {
    int N = A.size(); vector<T> temp = A; sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> C(N), last(temp.size(), -1); for (int i = 0; i < N; i++)
      C[i] = lower_bound(temp.begin(), temp.end(), A[i]) - temp.begin();
    for (int i = 0; i < N; i++) {
      int j = last[C[i]]; last[C[i]] = i; C[i] = j;
    }
    return MergeSortTree<int>(C.begin(), C.end());
  }
  CountDistinctOnlineMergeSortTree(const vector<T> &A) : mst(init(A)) {}
  int query(int l, int r) { return mst.rank(l, r, l); }
};
