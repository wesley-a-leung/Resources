#pragma once
#include <bits/stdc++.h>
using namespace std;

// Finds the minimum lexicographical rotation of a string
// Template Arguments:
//   It: the type of the iterator point to each character/element in
//     the string/array
// Function Arguments:
//   st: an iterator pointing to the first element in the string/array
//   en: an iterator pointing to after the last element in the string/array
// Return Value: returns an iterator to the first element of the minimum
//   lexicographical rotation of the string/array
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(1)
// Tested:
//   https://cses.fi/problemset/task/1110/
template <class It> It minRotation(It st, It en) {
  int N = en - st, i = 0;
  auto ind = [&] (int i) { return i < N ? i : i - N; };
  for (int j = 0; j < N; j++) for (int k = 0; k < N; k++) {
    auto &a = st[ind(i + k)], &b = st[ind(j + k)];
    if (i + k == j || a < b) { j += max(0, k - 1); break; }
    if (a > b) { i = j; break; }
  }
  return st + i;
}
