#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the multisubset sum problem with the occurrence of each sum (elements
//   can be chosen multiple times)
// Template Arguments:
//   U: the type of the count
//   T: the type of each element in the array
// Function Arguments:
//   A: a vector of type T
//   M: the maximum sum to count the number of multisubsets
// Return Value: a vector dp of type U with length M + 1 with dp[i] equal to
//   the number of multisubsets that have a sum of i
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(M)
// Tested:
//   https://cses.fi/problemset/task/1636
template <class U, class T>
vector<U> multisubsetSumCount(const vector<T> &A, int M) {
  vector<U> dp(M + 1, U()); dp[0] = U(1); for (auto &&a : A)
    for (int j = a; j <= M; j++) dp[j] += dp[j - a];
  return dp;
}

// Solves the multisubset sum problem (elements can be chosen multiple times)
// Template Arguments:
//   U: the type representing the boolean
//   T: the type of each element in the array
// Function Arguments:
//   A: a vector of type T
//   M: the maximum sum to determine if a subset exists
// Return Value: a boolean vector dp with length M + 1 with dp[i] equal to
//   1 if a multisubset has a sum of i, 0 otherwise
// In practice, has a very small constant, faster if U is int, but less memory
//   is used if U is bool (as long as the compiler optimizes vector<booL>);
//   char gives mixed results depending on whether vector<char> is compiled
//   as a string (better if it is not a string)
// Time Complexity: O(NM)
// Memory Complexity: O(M / 64)
// Tested:
//   Stress Tested
//   https://mcpt.ca/problem/acoinproblem
template <class U = bool, class T>
vector<U> multisubsetSum(const vector<T> &A, int M) {
  vector<U> dp(M + 1, U(0)); dp[0] = U(1); for (auto &&a : A)
    for (int j = a; j <= M; j++) dp[j] = dp[j] || dp[j - a];
  return dp;
}
