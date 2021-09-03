#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the minimum lexicographical rotation of an array
// Template Arguments:
//   T: the type of each element in the array
// Function Arguments:
//   S: a vector of type T
// Return Value: the index of the first element of the minimum
//   lexicographical rotation of the array
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://cses.fi/problemset/task/1110/
template <class T> int minRotation(const vector<T> &S) {
  int N = S.size(), i = 0;
  auto ind = [&] (int i) { return i < N ? i : i - N; };
  for (int j = 0; j < N; j++) for (int k = 0; k < N; k++) {
    auto &a = S[ind(i + k)], &b = S[ind(j + k)];
    if (i + k == j || a < b) { j += max(0, k - 1); break; }
    if (a > b) { i = j; break; }
  }
  return i;
}
