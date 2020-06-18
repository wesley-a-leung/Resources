#pragma once
#include <bits/stdc++.h>
using namespace std;

// If ISFIRST is true:
//   Returns the first value in the range [lo, hi) where f(x) is true
//     if no value in [lo, hi) satisfies f(x), then it returns hi
//     assumes that all values where f(x) is true are greater than all values
//     where f(x) is false
// IF ISFIRST is false:
//   Returns the last value in the range [lo, hi) where f(x) is true
//     if no value in [lo, hi) satisfies f(x), then it returns lo - 1
//     assumes that all values where f(x) is true are less than all values
//     where f(x) is false
// In practice, has a small constant
// Time Complexity: O(log (hi - lo)) * (cost to compute f(x))
// Memory Complexity: O(1)
// Tested:
//   https://mcpt.ca/problem/lcc18c5s3
//   https://dmoj.ca/problem/apio19p3
//   https://dmoj.ca/problem/pib20p2
//   https://dmoj.ca/problem/coci17c1p5
//   https://dmoj.ca/problem/dmopc18c5p5
//   https://www.spoj.com/problems/MKTHNUM/
const bool FIRST = true, LAST = false;
template <const bool ISFIRST, class T, class F> T bsearch(T lo, T hi, F f) {
  hi--; while (lo <= hi) {
    T mid = lo + (hi - lo) / 2;
    if (f(mid) == ISFIRST) hi = mid - 1;
    else lo = mid + 1;
  }
  return ISFIRST ? lo : hi;
}
