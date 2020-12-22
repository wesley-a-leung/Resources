#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum sum of a non empty subarray
// Template Arguments:
//   It: the type of the iterator for the array
//   T: the type of each value
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   NEG_INF: a value for negative infinity
// Return Value: a tuple with the maximum sum of a non empty subarray, an
//   iterator pointer to the start of that subarray, and an iterator pointing
//   to after the end of the subarray
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://mcpt.ca/problem/subarraymaximization
template <class It, class T = typename iterator_traits<It>::value_type>
tuple<T, It, It> maxSubarraySum(It st, It en,
                                  T NEG_INF = numeric_limits<T>::lowest()) {
  tuple<T, It, It> ret = make_tuple(NEG_INF, st, st); T sm = T();
  for (It cur = st, l = st; cur != en; cur++) {
    sm += *cur; if (get<0>(ret) < sm) ret = make_tuple(sm, l, cur + 1);
    if (sm < T()) { sm = T(); l = cur + 1; }
  }
  return ret;
}
