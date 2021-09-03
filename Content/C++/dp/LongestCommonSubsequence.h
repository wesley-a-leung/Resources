#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the longest common subsequence between 2 arrays
// Template Arguments:
//   T: the type of the elements in the arrays
// Function Arguments:
//   A: the first array
//   B: the second array
// Return Value: the length of the longest common subsequence between the
//   2 arrays
// In practice, has a small constant
// Time Complexity: O(NM) where N = en1 - st1 and M = en2 - st2
// Memory Complexity: O(M)
// Tested:
//   https://dmoj.ca/problem/lcs
template <class T>
int longestCommonSubsequence(const vector<T> &A, const vector<T> &B) {
  int N = A.size(), M = B.size();
  vector<vector<int>> dp(2, vector<int>(M + 1, 0));
  for (int i = 1; i <= N; i++) {
    int cur = i % 2, prv = 1 - cur; for (int j = 1; j <= M; j++)
      dp[cur][j] = A[i - 1] == B[j - 1] ? dp[prv][j - 1] + 1
                                        : max(dp[cur][j - 1], dp[prv][j]);
  }
  return dp[N % 2][M];
}
