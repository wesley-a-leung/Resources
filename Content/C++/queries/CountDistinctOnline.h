#pragma once
#include <bits/stdc++.h>
#include "../datastructures/WaveletMatrix.h"
using namespace std;

// Supports online queries for the number of distinct elements in the
//   range [l, r] for a static array A of length N
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   A: a vector of type T of the values in the array
// Functions:
//   query(l, r): returns the number of distinct values in the range [l, r]
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N log N)
//   query: O(log N)
// Memory Complexity: O((N log N) / 64)
// Tested:
//   https://www.acmicpc.net/problem/14898
template <class T> struct CountDistinctOnline {
  int N; WaveletMatrix<int> wm;
  WaveletMatrix<int> init(const vector<T> &A) {
    vector<T> temp = A; sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> last(temp.size(), -1); int i = 0;
    return WaveletMatrix<int>(N, [&] {
      int c = lower_bound(temp.begin(), temp.end(), A[i]) - temp.begin();
      int ret = last[c]; last[c] = i++; return ret;
    });
  }
  CountDistinctOnline(const vector<T> &A) : N(A.size()), wm(init(A)) {}
  int query(int l, int r) { return wm.rank(l, r, l); }
};
