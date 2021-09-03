#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the 0-1 knapsack problem (each item can appear either 0 or 1 times)
// Template Arguments:
//   V: the value type
// Function Arguments:
//   A: a vetor of pairs, with the first element of each pair being the weight
//     of type int, and the second being the value of type V
//   M: type maximum weight the knapsack can hold
//   NEG_INF: a value of type V for negative infinity
// Return Value: a vector dp of size M + 1 with dp[i] being the maximum value
//   that a knapsack with weights summing to exactly i has (or NEG_INF if
//   that sum is not possible)
// In practice, has a very small constant
// Time Complexity: O(NM)
// Memory Complexity: O(M)
// Tested:
//   https://atcoder.jp/contests/dp/tasks/dp_d
template <class V>
vector<V> zeroOneKnapsack(const vector<pair<int, V>> &A, int M,
                          V NEG_INF = numeric_limits<V>::lowest()) {
  vector<V> dp(M + 1, NEG_INF); dp[0] = V(); for (auto &&a : A)
    for (int j = M; j >= a.first; j--) if (dp[j - a.first] > NEG_INF)
      dp[j] = max(dp[j], dp[j - a.first] + a.second);
  return dp;
}

// Solves the dual of the 0-1 knapsack problem (each item can appear
//   either 0 or 1 times)
// Template Arguments:
//   W: the weight type
// Function Arguments:
//   A: a vetor of pairs, with the first element of each pair being the weight
//     of type W, and the second being the value of type int
//   K: type maximum value the knapsack can hold
//   INF: a value of type W for infinity
// Return Value: a vector dp of size K + 1 with dp[i] being the minimum weight
//   that a knapsack with values summing to exactly i has (or INF if that sum
//   is not possible)
// In practice, has a very small constant
// Time Complexity: O(NK)
// Memory Complexity: O(K)
// Tested:
//   https://atcoder.jp/contests/dp/tasks/dp_e
template <class W> vector<W> zeroOneKnapsackDual(
    const vector<pair<W, int>> &A, int K, W INF = numeric_limits<W>::max()) {
  vector<W> dp(K + 1, INF); dp[0] = W(); for (auto &&a : A)
    for (int j = K; j >= a.second; j--) if (dp[j - a.second] < INF)
      dp[j] = min(dp[j], dp[j - a.second] + a.first);
  return dp;
}
