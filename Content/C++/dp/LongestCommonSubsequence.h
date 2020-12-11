#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest common subsequence between 2 arrays
// Template Arguments:
//   It1: the type of the iterator for the first array
//   It2: the type of the iterator for the second array
// Function Arguments:
//   st1: an iterator pointing to the first element in the
//     first array
//   en1: an iterator pointing to after the last element in the
//     first array
//   st2: an iterator pointing to the first element in the
//     second array
//   en2: an iterator pointing to after the last element in the
//     second array
// Return Value: the length of the longest common subsequence between the
//   2 arrays
// In practice, has a small constant
// Time Complexity: O(NM) where N = en1 - st1 and M = en2 - st2
// Memory Complexity: O(M)
// Tested:
//   https://dmoj.ca/problem/lcs
template <class It1, class It2>
int longestCommonSubsequence(It1 st1, It1 en1, It2 st2, It2 en2) {
  int N = en1 - st1, M = en2 - st2;
  vector<vector<int>> dp(2, vector<int>(M + 1, 0));
  for (int i = 1; i <= N; i++) {
    int cur = i % 2, prv = 1 - cur; for (int j = 1; j <= M; j++)
      dp[cur][j] = st1[i - 1] == st2[j - 1] ? dp[prv][j - 1] + 1
                                            : max(dp[cur][j - 1], dp[prv][j]);
  }
  return dp[N % 2][M];
}
