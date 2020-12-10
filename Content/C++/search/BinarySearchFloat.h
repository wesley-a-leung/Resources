#pragma once
#include <bits/stdc++.h>
using namespace std;

// Binary search a boolean function over a floating point range
// Template Arguments:
//   T: the type of the range to search over, must be a floating point type
//   F: the type of the function that is being searched over
// Function Arguments:
//   lo: the inclusive lower bound
//   hi: the inclusive upper bound
//   f: the function to search over
//   iters: the number of iterations to run the ternary search
// Return Value: the value x in the range [lo, hi] such that f(x - EPS) is
//   false, while f(x + EPS) is true for some small value EPS
// In practice, has a small constant
// Time Complexity: O(iters) * (cost to compute f(x))
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/globexcup19s2
template <class T, class F> T bsearchFloat(T lo, T hi, F f, int iters) {
  static_assert(is_floating_point<T>::value,
                "T must be a floating point type");
  for (int it = 0; it < iters; it++) {
    T mid = lo + (hi - lo) / 2;
    if (f(mid)) hi = mid;
    else lo = mid;
  }
  return lo + (hi - lo) / 2;
}
