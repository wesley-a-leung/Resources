#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes base to the power of pow
// Template Arguments:
//   T: the type of base
//   U: the type of pow
// Function Arguments:
//   base: the base
//   pow: the power, must be non negative and integral
// Return Value: returns base to the power of pow
// In practice, has a small constant
// Time Complexity: O(log pow)
// Memory Complexity: O(1)
// Tested:
//   https://cses.fi/problemset/task/1095
template <class T, class U> T pow2(T base, U pow) {
  T x = 1; while (true) {
    if (pow % 2 == 1) x = x * base;
    if ((pow /= 2) == 0) break;
    base = base * base;
  }
  return x;
}
