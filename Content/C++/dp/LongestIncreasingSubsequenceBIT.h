#pragma once
#include <bits/stdc++.h>
#include "../datastructures/trees/fenwicktrees/FenwickTreeCumulative1D.h"
using namespace std;

// Computes a longest increasing subsequence in an array
// Template Arguments:
//   It: the type of the iterator for the array
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Return Value: a vector of indices of the element in the longest increasing
//   subsequence of the array, guaranteed the the values of those indices form
//   the lexicographically least subsequence
// In practice, has a small constant, slower than
//   LongestIncreasingSubsequence.h
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/lis
//   https://open.kattis.com/problems/longincsubseq
template <class It> vector<int> longestIncreasingSubsequenceBIT(It st, It en) {
  vector<typename iterator_traits<It>::value_type> tmp(st, en);
  int N = en - st, mx = 0; sort(tmp.begin(), tmp.end());
  tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
  vector<int> ret(N), ind(N + 1, -1); for (int i = 0; i < N; i++)
    ret[i] = lower_bound(tmp.begin(), tmp.end(), st[i]) - tmp.begin();
  struct Max { int operator () (int a, int b) { return max(a, b); } };
  FenwickTreeCumulative1D<int, Max> FT(N, 0); for (int i = 0; i < N; i++) {
    int len = FT.query(ret[i] - 1) + 1; if (ind[len] == -1) ind[mx = len] = i;
    else if (!(st[ind[len]] < st[i])) ind[len] = i; 
    FT.update(ret[i], len); ret[i] = ind[len - 1];
  }
  for (int i = N, k = ind[mx], p; k != -1; k = p) { p = ret[k]; ret[--i] = k; }
  ret.erase(ret.begin(), ret.end() - mx); return ret;
}
