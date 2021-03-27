#pragma once
#include <bits/stdc++.h>
#include "FenwickTree1D.h"
#include "../../../math/LagrangePolynomialInterpolation.h"
using namespace std;

// FenwickTree supporting range updates with updates in the form of
//   adding v, 2^k v, 3^k v, ... to the range [l, r], and range sum queries
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   N: the size of the array
// Functions:
//   update(l, r, v, k): add v, 2^k v, 3^k v, ... to the range [l, r]
//   query(r): queries the range [0, r]
//   query(l, r): queries the range [l, r]
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor: O(N + K^3)
//   update: O(K (K + log N))
//   rsq: O(K log N)
// Memory Complexity: O(KN + K^3)
// Tested:
//   https://dmoj.ca/problem/tle17c3p6
template <class T> struct FenwickTreePowerSum {
  int N, K; vector<FenwickTree1D<T>> FT; vector<T> fact;
  vector<vector<T>> cof, pascal; vector<vector<vector<T>>> sumDiff;
  FenwickTreePowerSum(int N, int K)
      : N(N), K(K), FT(K + 2, FenwickTree1D<T>(N)), fact(K + 2, T(1)),
        cof(K + 1), pascal(K + 2), sumDiff(K + 1, vector<vector<T>>(K + 2)) {
    for (int i = 1; i < K + 2; i++) fact.push_back(fact.back() * T(i));
    for (int i = 0; i < K + 1; i++) {
      vector<pair<T, T>> P; P.reserve(i + 2); P.emplace_back(T(), T());
      for (int j = 1; j < i + 2; j++) {
        T p = T(1); for (int k = 1; k <= i; k++) p *= T(j);
        P.emplace_back(T(j), P.back().second + fact[i + 1] * p);
      }
      cof[i] = lagrangePolynomialInterpolation(P);
    }
    for (int k = 0; k < K + 1; k++) for (int i = 0; i < K + 2; i++)
      sumDiff[k][i].resize(K + 2 - i, T());
    for (int i = 0; i < K + 2; i++) {
      pascal[i].resize(i + 1, T(1)); for (int j = 1; j < i; j++)
        pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
    }
    for (int k = 0; k < K + 1; k++) for (int i = 0; i < k + 2; i++) {
      T sign = T(1); for (int j = 0; j <= i; j++, sign = -sign) {
        T f = fact[K + 1] / fact[k + 1];
        sumDiff[k][i - j][j] += cof[k][i] * pascal[i][j] * f * sign;
      }
    }
  }
  void update(int l, int r, T v, int k) {
    assert(k <= K); T s = T(l - 1), len = T(r - l + 1);
    for (int i = 0; i < k + 2; i++) {
      T mul = sumDiff[k][i][k + 1 - i];
      for (int j = k - i; j >= 0; j--) mul = mul * s + sumDiff[k][i][j];
      FT[i].update(l, v * mul); FT[i].update(r + 1, -v * mul);
    }
    T mul = cof[k][k + 1];
    for (int i = k; i >= 0; i--) mul = mul * len + cof[k][i];
    mul = mul * (fact[K + 1] / fact[k + 1]); FT[0].update(r + 1, v * mul);
  }
  T query(int r) {
    T ret = FT[K + 1].query(r);
    for (int i = K; i >= 0; i--) ret = ret * r + FT[i].query(r);
    return ret / fact[K + 1];
  }
  T query(int l, int r) { return query(r) - query(l - 1); }
};
