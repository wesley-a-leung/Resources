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
// In practice, has a small constant
// Time Complexity: O(NM)
// Memory Complexity: O(M)
// Tested:
//   https://dmoj.ca/problem/knapsack
template <class It,
          class V = typename tuple_element<
              1, typename iterator_traits<It>::value_type>::type>
vector<V> boundedKnapsack(It st, It en, int M,
                          V NEG_INF = numeric_limits<V>::lowest()) {
  vector<V> dp(M + 1, NEG_INF), q(M + 1, V()), dq(M + 1, V()); dp[0] = V();
  for (It cur = st; cur != en; cur++) {
    int w = get<0>(*cur), f = get<2>(*cur); V v = get<1>(*cur);
    if (w <= M) for (int s = 0; s < w; s++) {
      V alpha = V(); int ql = 0, qr = 0, dql = 0, dqr = 0;
      for (int j = s; j <= M; j += w) {
        alpha += v; V a = dp[j] <= NEG_INF ? NEG_INF : dp[j] - alpha;
        while (dql < dqr && dq[dqr - 1] < a) dqr--;
        q[qr++] = dq[dqr++] = a;
        while (qr - ql > f + 1) if (q[ql++] == dq[dql]) dql++;
        dp[j] = dq[dql] <= NEG_INF ? NEG_INF : dq[dql] + alpha;
      }
    }
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
// In practice, has a small constant
// Time Complexity: O(NK)
// Memory Complexity: O(K)
// Tested:
//   https://dmoj.ca/problem/knapsack (Subtask 1)
template <class It,
          class W = typename tuple_element<
              0, typename iterator_traits<It>::value_type>::type>
vector<W> boundedKnapsackDual(It st, It en, int K,
                              W INF = numeric_limits<W>::max()) {
  vector<W> dp(K + 1, INF), q(K + 1, W()), dq(K + 1, W()); dp[0] = W();
  for (It cur = st; cur != en; cur++) {
    int v = get<1>(*cur), f = get<2>(*cur); W w = get<0>(*cur);
    if (v <= K) for (int s = 0; s < v; s++) {
      W alpha = W(); int ql = 0, qr = 0, dql = 0, dqr = 0;
      for (int j = s; j <= K; j += v) {
        alpha += w; W a = dp[j] >= INF ? INF : dp[j] - alpha;
        while (dql < dqr && dq[dqr - 1] > a) dqr--;
        q[qr++] = dq[dqr++] = a;
        while (qr - ql > f + 1) if (q[ql++] == dq[dql]) dql++;
        dp[j] = dq[dql] >= INF ? INF : dq[dql] + alpha;
      }
    }
  }
  return dp;
}
