#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds a candidate for the majority element with constant space
// A second pass is required to ensure the candidate is actually a
//   majority element
// Template Arguments:
//   F: the type of the function generating the elements
// Function Arguments:
//   N: the number of elements
//   f: a function that returns the ith element on the ith call
// Return Value: returns an element that is a candidate for the
//   majority element
// In practice, has a very small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/farmingmars
template <class F>
auto boyerMooreMajority(int N, F f) -> typename decay<decltype(f())>::type {
  typename decay<decltype(f())>::type ret = f();
  for (int cnt = 1, i = 1; i < N; i++) {
    auto v = f(); if (cnt == 0) { ret = v; cnt++; }
    else if (v == ret) cnt++;
    else cnt--;
  }
  return ret;
}
