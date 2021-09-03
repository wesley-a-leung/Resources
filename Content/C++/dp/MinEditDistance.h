#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum edit distance between 2 arrays, allowing for custom
//   penalties for copying, replacement, insertion, and deletion
// Template Arguments:
//   T: the type of each element in the arrays
//   U: the type of the penalties
// Function Arguments:
//   A: the first array of type T
//   B: the second array of type T
// Return Value: the minimum cost to edit the first array to become the
//   second array
// In practice, has a small constant
// Time Complexity: O(NM)
// Memory Complexity: O(M)
// Tested:
//   https://www.spoj.com/problems/EDIST/
template <class T, class U = int>
U minEditDistance(const vector<T> &A, const vector<T> &B, U cpyPen = U(),
    U repPen = U(1), U insPen = U(1), U delPen = U(1)) {
  int N = A.size(), M = B.size();
  vector<vector<U>> dp(2, vector<U>(M + 1, U()));
  for (int i = 1; i <= M; i++) dp[0][i] = dp[0][i - 1] + insPen;
  for (int i = 1; i <= N; i++) {
    int cur = i % 2, prv = 1 - cur; dp[cur][0] = dp[prv][0] + delPen;
    for (int j = 1; j <= M; j++) {
      dp[cur][j] = min(min(dp[prv][j - 1] + repPen, dp[cur][j - 1] + insPen),
                       dp[prv][j] + delPen);
      if (A[i - 1] == B[j - 1])
        dp[cur][j] = min(dp[cur][j], dp[prv][j - 1] + cpyPen);
    }
  }
  return dp[N % 2][M];
}
