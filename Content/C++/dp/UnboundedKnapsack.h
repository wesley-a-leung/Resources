#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the unbounded knapsack problem (each item can appear any number
//   of times)
// Template Arguments:
//   It: the type of the iterator for the array of pairs
//     with the first element being the weight of type int, and the second
//     being the value of type V
//   V: the value type
// Function Arguments:
//   st: an iterator pointing to the first element in the array of pairs
//     with the first element being the weight of type int, and the second
//     being the value of type V
//   en: an iterator pointing to after the last element in the array of pairs
//     with the first element being the weight of type int, and the second
//     being the value of type V
//   M: type maximum weight the knapsack can hold
//   INF: a value of type V for infinity
// Return Value: a vector dp of size M + 1 with dp[i] being the maximum value
//   that a knapsack with weights summing to exactly i has (or -INF if that sum
//   is not possible)
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(M)
// Tested:
//   https://www.spoj.com/problems/DBALLZ/
template <class It,
          class V = typename iterator_traits<It>::value_type::second_type>
vector<V> unboundedKnapsack(It st, It en, int M,
                            V INF = numeric_limits<V>::max()) {
  vector<V> dp(M + 1, -INF); dp[0] = V();
  for (It cur = st; cur != en; cur++) for (int j = 0; j <= M; j++)
    if (cur->first <= j && dp[j - cur->first] > -INF)
      dp[j] = max(dp[j], dp[j - cur->first] + cur->second);
  return dp;
}

// Solves the dual of the unbounded knapsack problem (each item can appear
//   any number of times)
// Template Arguments:
//   It: the type of the iterator for the array of pairs
//     with the first element being the weight of type int, and the second
//     being the value of type V
//   W: the weight type
// Function Arguments:
//   st: an iterator pointing to the first element in the array of pairs
//     with the first element being the weight of type W, and the second
//     being the value of type int
//   en: an iterator pointing to after the last element in the array of pairs
//     with the first element being the weight of type W, and the second
//     being the value of type int
//   K: type maximum value the knapsack can hold
//   INF: a value of type W for infinity
// Return Value: a vector dp of size K + 1 with dp[i] being the minimum weight
//   that a knapsack with values summing to exactly i has (or INF if that sum
//   is not possible)
// In practice, has a very small constant
// Time Complexity: O(NK)
// Memory Complexity: O(K)
// Tested:
//   https://www.spoj.com/problems/PIGBANK/
template <class It,
          class W = typename iterator_traits<It>::value_type::first_type>
vector<W> unboundedKnapsackDual(It st, It en, int K,
                               W INF = numeric_limits<W>::max()) {
  vector<W> dp(K + 1, INF); dp[0] = W();
  for (It cur = st; cur != en; cur++) for (int j = 0; j <= K; j++)
    if (cur->second <= j && dp[j - cur->second] < INF)
      dp[j] = min(dp[j], dp[j - cur->second] + cur->first);
  return dp;
}
