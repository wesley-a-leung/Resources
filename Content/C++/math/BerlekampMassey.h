#pragma once
#include <bits/stdc++.h>
using namespace std;

// Given a sequence A of length N, compute a sequence C of minimum length M,
//   such that A[i] = sum (C[j] * A[i - 1 - j]) for 0 <= j < M
// Template Arguments:
//   T: the type of the elements in the sequence
// Function Arguments:
//   A: the sequence to find the recurrence
// Return Value: the shortest vector C of length M, representing the
//   recurrence coefficients
// Time Complexity: O(N^2)
// Memory Complexity: O(N)
// Tested:
//   https://judge.yosupo.jp/problem/find_linear_recurrence
//   https://dmoj.ca/problem/ddrp6
template <class T> vector<T> berlekampMassey(const vector<T> &A) {
  if (A.empty()) return vector<T>();
  int N = A.size(), L = 0; vector<T> C(N, T()), B = C, tmp;
  T b = C[0] = B[0] = T(1); for (int i = 0, m = 1; i < N; i++, m++) {
    T d = A[i]; for (int j = 1; j <= L; j++) d += C[j] * A[i - j];
    if (d == T()) continue;
    tmp = C; T c = d / b; for (int j = m; j < N; j++) C[j] -= c * B[j - m];
    if (2 * L > i) continue;
    L = i + 1 - L; B = tmp; b = d; m = 0;
  }
  C.resize(L + 1); C.erase(C.begin());
  for (auto &&c : C) c = -c;
  return C;
}
