#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the multisubset sum problem with the occurence of each sum (elements
//   can be chosen multiple times)
// Template Arguments:
//   T: the type of the count
//   It: the type of the iterator for the array
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   M: the maximum sum to count the number of multisubsets
// Return Value: a vector dp of type T with length M + 1 with dp[i] equal to
//   the number of multisubsets that have a sum of i
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(M)
// Tested:
//   https://cses.fi/problemset/task/1636
template <class T, class It>
vector<T> multisubsetSumCount(It st, It en, int M) {
  vector<T> dp(M + 1, T()); dp[0] = T(1); for (It cur = st; cur != en; cur++)
    for (int j = *cur; j <= M; j++) dp[j] += dp[j - *cur];
  return dp;
}

// Solves the multisubset sum problem (elements can be chosen multiple times)
// Template Arguments:
//   T: the type representing the boolean
//   It: the type of the iterator for the array
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   M: the maximum sum to determine if a subset exists
// Return Value: a vector dp of type T with length M + 1 with dp[i] equal to
//   1 if a subset has a sum of i, 0 otherwise
// In practice, has a very small constant, faster if T is int, but less memory
//   is used if T is bool (as long as the compiler optimizes vector<booL>);
//   char gives mixed results depending on whether vector<char> is compiled
//   as a string (better if it is not a string)
// Time Complexity: O(NM)
// Memory Complexity: O(M), dependent on size of vector<T>(M + 1)
// Tested:
//   Stress Tested
//   https://mcpt.ca/problem/acoinproblem
template <class T = bool, class It>
vector<T> multisubsetSum(It st, It en, int M) {
  vector<T> dp(M + 1, 0); dp[0] = 1; for (It cur = st; cur != en; cur++)
    for (int j = *cur; j <= M; j++) dp[j] = dp[j] || dp[j - *cur];
  return dp;
}
