#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes a longest increasing subsequence in an array
// Template Arguments:
//   It: the type of the iterator for the array
// Function Arguments:
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Return Value: a vector of indices of the element in a longest increasing
//   subsequence of the array, the indices is guaranteed to be the
//   lexicographically largest vector of indices
// In practice, has a very small constant, faster than
//   LongestIncreasingSubsequenceBIT.h
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/lis
//   https://open.kattis.com/problems/longincsubseq
template <class It> vector<int> longestIncreasingSubsequence(It st, It en) {
  int N = en - st, k = -1; vector<int> ret(N);
  vector<pair<typename iterator_traits<It>::value_type, int>> dp;
  dp.reserve(N); for (int i = 0; i < N; i++) {
    int j = lower_bound(dp.begin(), dp.end(), make_pair(st[i], -1))
        - dp.begin();
    ret[i] = j == 0 ? -1 : dp[j - 1].second;
    if (j >= int(dp.size())) dp.emplace_back(st[i], i);
    else dp[j] = make_pair(st[i], i);
    if (j == int(dp.size()) - 1) k = i;
  }
  for (int i = N, p; k != -1; k = p) { p = ret[k]; ret[--i] = k; }
  ret.erase(ret.begin(), ret.end() - dp.size()); return ret;
}
