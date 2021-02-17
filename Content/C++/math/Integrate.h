#pragma once
#include <bits/stdc++.h>
using namespace std;

// Helper functions
template <class F, class T> T S(F &f, T a, T b) {
  return (f(a) + 4 * f((a + b) / 2) + f(b)) * (b - a) / 6;
}

template <class F, class T> T rec(F &f, T a, T b, T eps, T s) {
  T c = (a + b) / 2, S1 = S(f, a, c), S2 = S(f, c, b), SS = S1 + S2;
  if (abs(SS - s) <= 15 * eps || b - a < 1e-10) return SS + (SS - s) / 15;
  return rec(f, a, c, eps / 2, S1) + rec(f, c, b, eps / 2, S2);
}

// Integrates the function f over the range [a, b]
// Template Arguments:
//   F: the type of f
//   T: the type of the bounds and the return value of the function f
// Function Arguments:
//   a: the lower bound
//   b: the upper bound
//   f(x): the function returning the y value at x
//   eps: a value for epsilon
// Return Value: the integral of f over the range [a, b]
// Time Complexity: O(b - a) * (cost to compute f(x))
// Memory Complexity: O(1)
// Tested:
//   https://codeforces.com/problemsets/acmsguru/problem/99999/217
template <class F, class T>
T integrate(F f, T a, T b, T eps) { return rec(f, a, b, eps, S(f, a, b)); }
