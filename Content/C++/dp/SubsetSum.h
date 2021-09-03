#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the subset sum problem with the occurrence of each sum (elements
//   can be chosen 0 or 1 times)
// Template Arguments:
//   U: the type of the count
//   T: the type of each element in the array
// Function Arguments:
//   A: a vector of type T
//   M: the maximum sum to count the number of subsets
// Return Value: a vector dp of type U with length M + 1 with dp[i] equal to
//   the number of subsets that have a sum of i
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(M)
// Tested:
//   Fuzz Tested
template <class U, class T>
vector<U> subsetSumCount(const vector<T> &A, int M) {
  vector<U> dp(M + 1, U()); dp[0] = U(1); for (auto &&a : A)
    for (int j = M; j >= a; j--) dp[j] += dp[j - a];
  return dp;
}

// Solves the subset sum problem (elements can be chosen 0 or 1 times)
// Template Arguments:
//   M: the maximum sum to determine if a subset exists
//   T: the type of each element in the array
// Function Arguments:
//   A: a vector of type T
//   en: an iterator pointing to after the last element in the array
// Return Value: a bitset dp with length M + 1 with dp[i] equal to
//   whether a subset has a sum of i
// In practice, has a very small constant
// Time Complexity: O(NM / 64)
// Memory Complexity: O(M / 64)
// Tested:
//   Fuzz Tested
//   https://cses.fi/problemset/task/1745/
template <const int M, class T> bitset<M + 1> subsetSum(const vector<T> &A) {
  bitset<M + 1> dp; dp[0] = true; for (auto &&a : A) dp |= dp << a;
  return dp;
}
