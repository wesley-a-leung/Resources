#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of a non empty, non consecutive subsequence
// Template Arguments:
//   It: the type of the iterator for the array
//   T: the type of each value
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   NEG_INF: a value for negative infinity
// Return Value: the maximum sum of a non empty, non consecutive subsequence
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/seq0
template <class It, class T = typename iterator_traits<It>::value_type>
T maxNonConsecutiveSum(It st, It en, T NEG_INF = numeric_limits<T>::lowest()) {
  T incl = NEG_INF, excl = NEG_INF; for (It cur = st; cur != en; cur++) {
    T val = *cur; if (excl > NEG_INF) val = max(val, val + excl);
    excl = max(incl, excl); incl = val;
  }
  return max(incl, excl);
}
