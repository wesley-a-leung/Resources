#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes a longest increasing subsequence in an array
// Template Arguments:
//   T: the type of each element in the array
// Function Arguments:
//   A: the array
// Return Value: a vector of indices of the element in a longest increasing
//   subsequence of the array, the indices is guaranteed to be the
//   lexicographically largest vector of indices
// In practice, has a very small constant, faster than the Fenwick Tree version
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/lis
//   https://open.kattis.com/problems/longincsubseq
template <class T>
vector<int> longestIncreasingSubsequence(const vector<T> &A) {
  int N = A.size(), k = -1; vector<int> ret(N);
  vector<pair<T, int>> dp; dp.reserve(N); for (int i = 0; i < N; i++) {
    int j = lower_bound(dp.begin(), dp.end(), make_pair(A[i], -1))
        - dp.begin();
    ret[i] = j == 0 ? -1 : dp[j - 1].second;
    if (j >= int(dp.size())) dp.emplace_back(A[i], i);
    else dp[j] = make_pair(A[i], i);
    if (j == int(dp.size()) - 1) k = i;
  }
  for (int i = N, p; k != -1; k = p) { p = ret[k]; ret[--i] = k; }
  ret.erase(ret.begin(), ret.end() - dp.size()); return ret;
}
