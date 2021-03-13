#pragma once
#include <bits/stdc++.h>
#include "MaxRectAreaHistogram.h"
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
  int N = A.size(), M = N == 0 ? 0 : A[0].size(), ret = 0; vector<int> H(M, 0);
  for (int i = N - 1; i >= 0; i--) {
    for (int j = 0; j < M; j++) H[j] = A[i][j] ? i == N - 1 ? 1 : H[j] + 1 : 0;
    ret = max(ret, maxRectAreaHistogram(H));
  }
  return ret;
}
