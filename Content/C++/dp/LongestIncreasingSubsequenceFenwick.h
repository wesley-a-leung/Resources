#pragma once
#include <bits/stdc++.h>
#include "../datastructures/trees/fenwicktrees/FenwickTree1D.h"
using namespace std;

// Computes a longest increasing subsequence in an array
// Template Arguments:
//   T: the type of each element in the array
// Function Arguments:
//   A: a vector of type T
// Return Value: a vector of indices of the element in the longest increasing
//   subsequence of the array, guaranteed that the values of those indices form
//   the lexicographically least subsequence
// In practice, has a small constant, slower than
//   LongestIncreasingSubsequence.h
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/lis
//   https://open.kattis.com/problems/longincsubseq
template <class T>
vector<int> longestIncreasingSubsequenceFenwick(const vector<T> &A) {
  vector<T> tmp = A; int N = A.size(), mx = 0; sort(tmp.begin(), tmp.end());
  tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
  vector<int> ret(N), ind(N + 1, -1); for (int i = 0; i < N; i++)
    ret[i] = lower_bound(tmp.begin(), tmp.end(), A[i]) - tmp.begin();
  struct Max { int operator () (int a, int b) { return max(a, b); } };
  FenwickTree1D<int, Max> FT(N, 0); for (int i = 0; i < N; i++) {
    int len = FT.query(ret[i] - 1) + 1; if (ind[len] == -1) ind[mx = len] = i;
    else if (!(A[ind[len]] < A[i])) ind[len] = i; 
    FT.update(ret[i], len); ret[i] = ind[len - 1];
  }
  for (int i = N, k = ind[mx], p; k != -1; k = p) { p = ret[k]; ret[--i] = k; }
  ret.erase(ret.begin(), ret.end() - mx); return ret;
}
