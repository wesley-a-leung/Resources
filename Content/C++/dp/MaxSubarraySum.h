#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of a non empty subarray
// Template Arguments:
//   T: the type of each element in the array
// Function Arguments:
//   A: a vector of type T
//   NEG_INF: a value for negative infinity
// Return Value: a tuple with the maximum sum of a non empty subarray, and the
//   inclusive left and right indices of the subarray
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://mcpt.ca/problem/subarraymaximization
template <class T> tuple<T, int, int> maxSubarraySum(
    const vector<T> &A, T NEG_INF = numeric_limits<T>::lowest()) {
  tuple<T, int, int> ret = make_tuple(NEG_INF, 0, -1); T sm = T();
  for (int i = 0, l = 0; i < int(A.size()); i++) {
    sm += A[i]; if (get<0>(ret) < sm) ret = make_tuple(sm, l, i);
    if (sm < T()) { sm = T(); l = i + 1; }
  }
  return ret;
}
