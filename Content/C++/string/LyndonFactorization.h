#pragma once
#include <bits/stdc++.h>
using namespace std;

// Factorizes a string S into Lyndon words w1w2w3... where w1, w2, w3, ... are
//   in non-increasing order
// Guaranteed to exist and is unique
// A Lyndon word is lexicographically smaller than all of its nontrivial
//   suffixes
// Template Arguments:
//   It: the type of the iterator point to each character/element in
//     the string/array
// Function Arguments:
//   st: an iterator pointing to the first element in the string/array
//   en: an iterator pointing to after the last element in the string/array
// Return Value: a vector of integers representing the lengths of each factor,
//   where the sum of the values in the vector is equal to
//   the length of the string
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://cses.fi/problemset/task/1110/
template <class It> vector<int> lyndonFactorization(It st, It en) {
  int N = en - st; vector<int> ret; for (int i = 0; i < N;) {
    int j = i + 1, k = i;
    for (; j < N && st[k] <= st[j]; j++) k = st[k] < st[j] ? i : k + 1;
    for (; i <= k; i += j - k) ret.push_back(j - k);
  }
  return ret;
}
