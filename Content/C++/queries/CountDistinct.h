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
// In practice, has a moderate constant, faster than mo, but slower than the
//   offline version
// Time Complexity:
//   constructor: O(N log N)
//   query: O(log N)
// Memory Complexity: O(N + (N log N) / 64)
// Tested:
//   https://www.acmicpc.net/problem/14898
template <class T> struct CountDistinct {
  WaveletMatrix<int> wm;
  vector<int> init(const vector<T> &A) {
    vector<T> temp = A; sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> last(temp.size(), -1), ret(A.size());
    for (int i = 0; i < int(A.size()); i++) {
      int c = lower_bound(temp.begin(), temp.end(), A[i]) - temp.begin();
      ret[i] = last[c]; last[c] = i;
    }
    return ret;
  }
  CountDistinct(const vector<T> &A) : wm(init(A)) {}
  int query(int l, int r) { return wm.rank(l, r, l); }
};
