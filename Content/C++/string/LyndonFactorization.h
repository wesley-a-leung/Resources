#pragma once
#include <bits/stdc++.h>
using namespace std;

// Factorizes a array S into Lyndon words w1w2w3... where w1, w2, w3, ... are
//   in non-increasing order
// Guaranteed to exist and is unique
// A Lyndon word is lexicographically smaller than all of its nontrivial
//   suffixes
// Template Arguments:
//   T: the type of each element in the array
// Function Arguments:
//   S: a vector of type T
// Return Value: a vector of integers representing the lengths of each factor,
//   where the sum of the values in the vector is equal to
//   the length of the array
// In practice, has a small constant
// Time Complexity: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://cses.fi/problemset/task/1110/
template <class T> vector<int> lyndonFactorization(const vector<T> &S) {
  int N = S.size(); vector<int> ret; for (int i = 0; i < N;) {
    int j = i + 1, k = i;
    for (; j < N && S[k] <= S[j]; j++) k = S[k] < S[j] ? i : k + 1;
    for (; i <= k; i += j - k) ret.push_back(j - k);
  }
  return ret;
}
