#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of a non empty, non consecutive subsequence
// Template Arguments:
//   T: the type of each element in the array
// Function Arguments:
//   A: a vector of type T
//   NEG_INF: a value for negative infinity
// Return Value: the maximum sum of a non empty, non consecutive subsequence
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/seq0
template <class T> T maxNonConsecutiveSum(
    const vector<T> &A, T NEG_INF = numeric_limits<T>::lowest()) {
  T incl = NEG_INF, excl = NEG_INF; for (auto &&a : A) {
    T v = a; if (excl > NEG_INF) v = max(v, v + excl);
    excl = max(incl, excl); incl = v;
  }
  return max(incl, excl);
}
