#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum edit distance between 2 arrays, allowing for custom
//   penalties for copying, replacement, insertion, and deletion
// Template Arguments:
//   It1: the type of the iterator for the first array
//   It2: the type of the iterator for the second array
//   T: the type of the penalties
// Function Arguments:
//   st1: an iterator pointing to the first element in the
//     first array
//   en1: an iterator pointing to after the last element in the
//     first array
//   st2: an iterator pointing to the first element in the
//     second array
//   en2: an iterator pointing to after the last element in the
//     second array
//   cpyPen: the penalty for copying an element
//   repPen: the penalty for replacing an element
//   insPen: the penalty for inserting an element
//   delPen: the penalty for deleting an element
// Return Value: the minimum cost to edit the first array to become the
//   second array
// In practice, has a small constant
// Time Complexity: O(NM) where N = en1 - st1 and M = en2 - st2
// Memory Complexity: O(M)
// Tested:
//   https://www.spoj.com/problems/EDIST/
template <class It1, class It2, class T = int>
T minEditDistance(It1 st1, It1 en1, It2 st2, It2 en2, T cpyPen = T(),
    T repPen = T(1), T insPen = T(1), T delPen = T(1)) {
  int N = en1 - st1, M = en2 - st2;
  vector<vector<T>> dp(2, vector<T>(M + 1, T()));
  for (int i = 1; i <= M; i++) dp[0][i] = dp[0][i - 1] + insPen;
  for (int i = 1; i <= N; i++) {
    int cur = i % 2, prv = 1 - cur; dp[cur][0] = dp[prv][0] + delPen;
    for (int j = 1; j <= M; j++) {
      dp[cur][j] = min(min(dp[prv][j - 1] + repPen, dp[cur][j - 1] + insPen),
                       dp[prv][j] + delPen);
      if (st1[i - 1] == st2[j - 1])
        dp[cur][j] = min(dp[cur][j], dp[prv][j - 1] + cpyPen);
    }
  }
  return dp[N % 2][M];
}
