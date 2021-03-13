#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the maximum axis aligned rectangular area of a histogram where
//   all bars have width 1
// Template Arguments:
//   T: the type of the heights
// Function Arguments:
//   A: a vector of the heights of each bar
// Return Value: the maximum axis aligned rectangular area of the histogram
// In practice, has a moderate constant
// Time Complexity: O(N)
// Memory Complexity: O(N)
// Tested:
//   https://www.acmicpc.net/problem/6549
template <class T> T maxRectAreaHistogram(vector<T> A) {
  T ret = T(); int N = A.size(), top = 0; vector<int> stk(N);
  for (int i = 0; i < int(A.size()); i++) {
    int j = i; while (top > 0 && A[stk[top - 1]] >= A[i]) {
      ret = max(ret, (i - stk[top - 1]) * A[stk[top - 1]]);
      A[j = stk[--top]] = A[i];
    }
    stk[top++] = j;
  }
  for (; top > 0; top--) ret = max(ret, (N - stk[top - 1]) * A[stk[top - 1]]);
  return ret;
}
