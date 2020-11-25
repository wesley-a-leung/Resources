#pragma once
#include <bits/stdc++.h>
using namespace std;

// Given a set of N points, P[i] = (X[i], Y[i]), compute a polynomial Q of
//   degree N - 1, such that Q passes through them:
//   Q(x) = A[0] * x^0 + ... + A[N - 1] * x^(N - 1).
// For real numbers, pick X[i] = C * cos(i / (N - 1) * PI) for a
//   large constant C
// Template Arguments:
//   T: the type of the x and y values, as well as the coefficients
// Function Arguments:
//   P: a vector of pairs of type T representing the N points
// Return Value: a polynomial of degree N - 1 (with N coefficients) with
//   the ith coefficient associated with the term x^i
// In practice, has a small constant
// Time Complexity: O(N^2)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/angieandfunctions
template <class T>
vector<T> lagrangePolynomialInterpolation(vector<pair<T, T>> P) {
  int N = P.size(); vector<T> A(N, T()), temp(N, T()); temp[0] = T(1);
  for (int k = 0; k < N; k++) for (int i = k + 1; i < N; i++)
    P[i].second = (P[i].second - P[k].second) / (P[i].first - P[k].first);
  T last = T(); for (int k = 0; k < N; k++) for (int i = 0; i < N; i++) {
    A[i] += P[k].second * temp[i]; swap(last, temp[i]);
    temp[i] -= P[k].first * last;
  }
  return A;
}
