#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the Kth term of a linear recurrence of length N
//   A[i] = sum (C[j] * A[i - 1 - j]) for 0 <= j < N
// Template Arguments:
//   T: the type of the elements in the sequence
//   U: the type of K
// Function Arguments:
//   A: a vector of the first N terms of the sequence
//   C: a vector of the the N recurrence coefficients of the sequence
//   K: the term of the sequence to find
// Return Value: the Kth term (0-indexed) of the linear recurrence
// Time Complexity: O(N^2 log K)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ddrp6
template <class T, class U>
T linearRecurrence(const vector<T> &A, const vector<T> &C, U K) {
  assert(A.size() == C.size()); int N = A.size(); if (K < N) return A[K];
  auto comb = [&] (const vector<T> &a, const vector<T> &b) {
    vector<T> res(N * 2 + 1, T());
    for (int i = 0; i <= N; i++) for (int j = 0; j <= N; j++)
      res[i + j] += a[i] * b[j];
    for (int i = N * 2; i > N; i--) for (int j = 0; j < N; j++)
      res[i - 1 - j] += res[i] * C[j];
    return res;
  };
  vector<T> P(N + 1, T()), E = P; P[0] = E[1] = T(1); K++; while (true) {
    if (K % 2 == 1) P = comb(P, E);
    if ((K /= 2) == 0) break;
    E = comb(E, E);
  }
  T res = T(); for (int i = 0; i < N; i++) res += P[i + 1] * A[i];
  return res;
}
