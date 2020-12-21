#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the subset sum problem with the occurence of each sum (elements
//   can be chosen 0 or 1 times)
// Template Arguments:
//   T: the type of the count
//   It: the type of the iterator for the array
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   M: the maximum sum to count the number of subsets
// Return Value: a vector dp of type T with length M + 1 with dp[i] equal to
//   the number of subsets that have a sum of i
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(M)
// Tested:
//   Fuzz Tested
template <class T, class It> vector<T> subsetSumCount(It st, It en, int M) {
  vector<T> dp(M + 1, T()); dp[0] = T(1); for (It cur = st; cur != en; cur++)
    for (int j = M; j >= *cur; j--) dp[j] += dp[j - *cur];
  return dp;
}

// Solves the subset sum problem (elements can be chosen 0 or 1 times)
// Template Arguments:
//   M: the maximum sum to determine if a subset exists
//   It: the type of the iterator for the array
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Return Value: a bitset dp with length M + 1 with dp[i] equal to
//   whether a subset has a sum of i
// In practice, has a very small constant
// Time Complexity: O(NM / 64)
// Memory Complexity: O(M / 64)
// Tested:
//   Fuzz Tested
template <const int M, class It> bitset<M + 1> subsetSum(It st, It en) {
  bitset<M + 1> dp; dp[0] = true;
  for (It cur = st; cur != en; cur++) dp |= dp << *cur;
  return dp;
}
