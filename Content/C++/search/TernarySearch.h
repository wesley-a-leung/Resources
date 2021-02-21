#pragma once
#include <bits/stdc++.h>
using namespace std;

// Ternary search for the maximum of a function
// Template Arguments:
//   T: the type of the range to search over, must be a floating point type
//   F: the type of the function that is being searched over
//   Cmp: the comparator to compare two f(x) values,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Function Arguments:
//   lo: the inclusive lower bound
//   hi: the inclusive upper bound
//   f: the function to search over
//   iters: the number of iterations to run the ternary search
//   cmp: an instance of the Cmp class
// Return Value: the value x in the range [lo, hi] such that f(x) is
//   maximum based on the comparator
// In practice, has a small constant
// Time Complexity: O(iters) * (cost to compute f(x))
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/contest/578/problem/C
template <class T, class F, class Cmp = less<T>>
T tsearch(T lo, T hi, F f, int iters, Cmp cmp = less<T>()) {
  static_assert(is_floating_point<T>::value,
                "T must be a floating point type");
  for (int it = 0; it < iters; it++) {
    T m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
    if (cmp(f(m1), f(m2))) lo = m1;
    else hi = m2;
  }
  return lo + (hi - lo) / 2;
}
