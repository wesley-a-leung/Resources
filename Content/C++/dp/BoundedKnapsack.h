#pragma once
#include <bits/stdc++.h>
using namespace std;

// Solves the bounded knapsack problem (each item can appear up to a specified
//   number of times)
// Template Arguments:
//   It: the type of the iterator for the array of tuples
//     with the first element being the weight of type int, the second
//     being the value of type V, and the third being the frequency of type int
//   V: the value type
// Function Arguments:
//   st: an iterator pointing to the first element in the array of tuples
//     with the first element being the weight of type int, the second
//     being the value of type V, and the third being the frequency of type int
//   en: an iterator pointing to after the last element in the array of tuples
//     with the first element being the weight of type int, the second
//     being the value of type V, and the third being the frequency of type int
//   M: type maximum weight the knapsack can hold
//   NEG_INF: a value of type V for negative infinity
// Return Value: a vector dp of size M + 1 with dp[i] being the maximum value
//   that a knapsack with weights summing to exactly i has (or NEG_INF if
//   that sum is not possible)
// In practice, has a very small constant
// Time Complexity: O(NM log M)
// Memory Complexity: O(M)
// Tested:
//   https://dmoj.ca/problem/knapsack
template <class It,
          class V = typename tuple_element<
              1, typename iterator_traits<It>::value_type>::type>
vector<V> boundedKnapsack(It st, It en, int M,
                          V NEG_INF = numeric_limits<V>::lowest()) {
  vector<V> dp(M + 1, NEG_INF); dp[0] = V();
  auto update = [&] (int w, V v) {
    for (int j = M; j >= w; j--) if (dp[j - w] > NEG_INF)
      dp[j] = max(dp[j], dp[j - w] + v);
  };
  for (It cur = st; cur != en; cur++) {
    int w = get<0>(*cur), f = min(get<2>(*cur), M / w), sum = 0;
    V v = get<1>(*cur); for (int mul = 1; sum + mul < f; sum += mul, mul *= 2)
      update(mul * w, mul * v);
    update((f - sum) * w, (f - sum) * v);
  }
  return dp;
}

// Solves the dual of the bounded knapsack problem (each item can appear up to
//   a specified number of times)
// Template Arguments:
//   It: the type of the iterator for the array of tuples
//     with the first element being the weight of type int, the second
//     being the value of type V, and the third being the frequency of type int
//   W: the weight type
// Function Arguments:
//   st: an iterator pointing to the first element in the array of tuples
//     with the first element being the weight of type int, the second
//     being the value of type V, and the third being the frequency of type int
//   en: an iterator pointing to after the last element in the array of tuples
//     with the first element being the weight of type int, the second
//     being the value of type V, and the third being the frequency of type int
//   K: type maximum value the knapsack can hold
//   INF: a value of type W for infinity
// Return Value: a vector dp of size K + 1 with dp[i] being the minimum weight
//   that a knapsack with values summing to exactly i has (or INF if that sum
//   is not possible)
// In practice, has a very small constant
// Time Complexity: O(NK log K)
// Memory Complexity: O(K)
// Tested:
//   https://dmoj.ca/problem/knapsack (Subtask 1)
template <class It,
          class W = typename tuple_element<
              0, typename iterator_traits<It>::value_type>::type>
vector<W> boundedKnapsackDual(It st, It en, int K,
                              W INF = numeric_limits<W>::max()) {
  vector<W> dp(K + 1, INF); dp[0] = W();
  auto update = [&] (W w, int v) {
    for (int j = K; j >= v; j--) if (dp[j - v] < INF)
      dp[j] = min(dp[j], dp[j - v] + w);
  };
  for (It cur = st; cur != en; cur++) {
    int v = get<1>(*cur), f = min(get<2>(*cur), K / v), sum = 0;
    W w = get<0>(*cur); for (int mul = 1; sum + mul < f; sum += mul, mul *= 2)
      update(mul * w, mul * v);
    update((f - sum) * w, (f - sum) * v);
  }
  return dp;
}
