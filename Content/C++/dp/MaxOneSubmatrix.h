#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum submatrix of a boolean matrix where
//   all elements are true
// Function Arguments:
//   A: a vector of vector of booleans
// Return Value: the maximum submatrix of the boolean matrix where
//   all elements are true
// In practice, has a moderate constant
// Time Complexity: O(NM)
// Memory Complexity: O(NM)
// Tested:
//   https://dmoj.ca/problem/ccoprep16q1
int maxOneSubmatrix(const vector<vector<bool>> &A) {
  int N = A.size(), M = N == 0 ? 0 : A[0].size(), top = 0, ret = 0;
  vector<int> stk(M); vector<vector<int>> H(N, vector<int>(M, 0));
  for (int i = N - 1; i >= 0; i--) for (int j = 0; j < M; j++) if (A[i][j])
    H[i][j] = i == N - 1 ? 1 : H[i + 1][j] + 1;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      int k = j; while (top > 0 && H[i][stk[top - 1]] >= H[i][j]) {
        ret = max(ret, (j - stk[top - 1]) * H[i][stk[top - 1]]);
        H[i][k = stk[--top]] = H[i][j];
      }
      stk[top++] = k;
    }
    for (; top > 0; top--)
      ret = max(ret, (M - stk[top - 1]) * H[i][stk[top - 1]]);
  }
  return ret;
}
