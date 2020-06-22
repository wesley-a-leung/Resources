#pragma once
#include <bits/stdc++.h>
using namespace std;

// A collections of functions for 2d prefix sum and difference arrays

// Creates a prefix sum array pre from the 2d array A of size N x M
// pre[i][j] is the sum of all elements in the subarray A[0..i][0..j]
// Indices are 0-indexed
// Time Complexity: O(NM)
// Memory Complexity: O(1) additional memory
// Tested:
//   Fuzz and Stress Tested
//   https://codeforces.com/contest/635/problem/A
//   https://dmoj.ca/problem/dmpg15s5
template <class C> void partial_sum_2d(const C &A, int N, int M, C &pre) {
  for (int i = 0; i < N; i++) for (int j = 0; j < M; j++) {
    pre[i][j] = A[i][j];
    if (i > 0) pre[i][j] += pre[i - 1][j];
    if (j > 0) pre[i][j] += pre[i][j - 1];
    if (i > 0 && j > 0) pre[i][j] -= pre[i - 1][j - 1];
  }
}

// Creates a difference array diff from the 2d array A of size N x M
// A[i][j] is the sum of all elements in the subarray diff[0..i][0..j]
// Indices are 0-indexed
// Time Complexity: O(NM)
// Memory Complexity: O(1) additional memory
// Tested:
//   Fuzz and Stress Tested
template <class C>
void adjacent_difference_2d(const C &diff, int N, int M, C &A) {
  for (int i = N - 1; i >= 0; i--) for (int j = M - 1; j >= 0; j--) {
    A[i][j] = diff[i][j];
    if (i > 0) A[i][j] -= diff[i - 1][j];
    if (j > 0) A[i][j] -= diff[i][j - 1];
    if (i > 0 && j > 0) A[i][j] += diff[i - 1][j - 1];
  }
}

// Computes the sum of a subarray A[u..d][l..r] with prefix sum array pre
// Indices are 0-indexed and ranges are inclusive
// Time Complexity: O(1)
// Memory Complexity: O(1) additional memory
// Tested:
//   Fuzz and Stress Tested
//   https://codeforces.com/contest/635/problem/A
template <class C> auto rsq(const C &pre, int u, int d, int l, int r)
    -> typename decay<decltype(pre[d][r])>::type {
  typename decay<decltype(pre[d][r])>::type v = pre[d][r];
  if (u > 0) v -= pre[u - 1][r];
  if (l > 0) v -= pre[d][l - 1];
  if (u > 0 && l > 0) v += pre[u - 1][l - 1];
  return v;
}

// Adds v to the subarray A[u..d][l..r] with difference array diff
//   of size N x M
// Indices are 0-indexed and ranges are inclusive
// Time Complexity: O(1)
// Memory Complexity: O(1) additional memory
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/dmpg15s5
template <class C, class T>
void add(C &diff, int N, int M, int u, int d, int l, int r, T v) {
  diff[u][l] += v;
  if (d + 1 < N) diff[d + 1][l] -= v;
  if (r + 1 < M) diff[u][r + 1] -= v;
  if (d + 1 < N && r + 1 < M) diff[d + 1][r + 1] += v;
}
