#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the floor of the sqrt of an integer
// Template Arguments:
//   T: the type of x, must be integral
// Function Arguments:
//   x: the value to compute the sqrt of, must be non-negative
// Return Value: returns the floor of the sqrt of x
// In practice, has a moderate constant, slower than binary search, but this
//   does not overflow
// Time Complexity: O(log x)
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/gym/100753/problem/F
template <class T> T isqrt(T x) {
  static_assert(is_integral<T>::value, "T must be integral");
  assert(x >= 0); if (x <= 1) return x;
  T y = x, z = 0; while (y > z) z = x / (y = (y + z) / 2);
  return y;
}
