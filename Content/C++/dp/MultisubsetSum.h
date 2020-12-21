#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the multisubset sum problem (elements can be chosen multiple timess)
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
template <class T, class It> vector<T> multisubsetSum(It st, It en, int M) {
  vector<T> dp(M + 1, T()); dp[0] = T(1); for (It cur = st; cur != en; cur++)
    for (int j = *cur; j <= M; j++) dp[j] += dp[j - *cur];
  return dp;
}
