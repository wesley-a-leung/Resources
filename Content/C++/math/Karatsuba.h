#pragma once
#include <bits/stdc++.h>
using namespace std;

// Karatsuba Multiplication
// Template Arguments:
//   T: the type of each element
//   ItA: the type of the iterator pointing to the first multiplyPolynomial
//   ItB: the type of the iterator pointing to the second polynomial
//   ItRes: the type of the iterator pointing to the result polynomial
// Function Arguments:
//   n: the length of the polynomial, must be a power of 2
//   a: a pointer to the first polynomial of length n, a[i] stores
//     the coefficient of x^i
//   b: a pointer to the second polynomial of length n, b[i] stores
//     the coefficient of x^i
//   res: a pointer to the result polynomial of length 2n which stores the
//     result of a multipled by b, res[i] stores the coefficient of x^i
// In practice, has a small constant, slower than FFT and NTT
// Time Complexity: O(n^log_2(3))
// Memory Complexity: O(n)
// Tested:
//   https://open.kattis.com/problems/polymul2
//   https://dmoj.ca/problem/atimesb
const int KARATSUBA_CUTOFF = 16;
template <class T, class ItA, class ItB, class ItRes>
void karatsuba(int n, ItA a, ItB b, ItRes res) {
  if (n <= KARATSUBA_CUTOFF) {
    fill(res, res + n * 2, 0); for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++) res[i + j] += a[i] * b[j];
    return;
  }
  assert(!(n & (n - 1))); int k = n / 2; vector<T> tmp(n, T()), c(n, T());
  auto atmp = tmp.begin(), btmp = atmp + k; for (int i = 0; i < k; i++) {
    atmp[i] = a[i] + a[i + k]; btmp[i] = b[i] + b[i + k];
  }
  karatsuba<T>(k, atmp, btmp, c.begin()); karatsuba<T>(k, a, b, res);
  karatsuba<T>(k, a + k, b + k, res + n); for (int i = 0; i < k; i++) {
    T t = res[i + k]; res[i + k] += c[i] - res[i] - res[i + k * 2];
    res[i + k * 2] += c[i + k] - t - res[i + k * 3];
  }
}

// Polynomial Multiplication
// Template Arguments:
//   T: the type of each element
// Functions Arguments:
//   a: a vector of type T representing the first polynomial, a[i] stores
//     the coefficient of x^i
//   b: a vector of type T representing the second polynomial, b[i] stores
//     the coefficient of x^i
// Return Value: a vector of type T representing the polynomial a times b with
//   no trailing zeros
// In practice, has a small constant
// Time Complexity: O(n^log_2(3)) where n = max(size(a), size(b))
// Memory Complexity: O(n)
// Tested:
//   https://open.kattis.com/problems/polymul2
template <class T> vector<T> mulPolyKaratsuba(vector<T> a, vector<T> b) {
  int n = max(a.size(), b.size()); while (n & (n - 1)) n++;
  a.resize(n, T()); b.resize(n, T()); vector<T> res(n * 2, T());
  karatsuba<T>(n, a.begin(), b.begin(), res.begin());
  while (int(res.size()) > 1 && res.back() == T()) res.pop_back();
  return res;
}

// Integer Multiplication
// Template Arguments:
//   T: the type of each digit
// Functions Arguments:
//   a: a vector of type T representing the first integer, a[i] stores
//     the digit of BASE^i
//   b: a vector of type T representing the second polynomial, b[i] stores
//     the digit of BASE^i
//   BASE: the base of the integer
// Return Value: a vector of type T representing the integer a times b with
//   no trailing zeros
// In practice, has a small constant
// Time Complexity: O(n^log_2(3)) where n = max(size(a), size(b))
// Memory Complexity: O(n)
// Tested:
//   https://dmoj.ca/problem/atimesb
template <class T>
vector<T> mulIntKaratsuba(vector<T> a, vector<T> b, T BASE = T(10)) {
  vector<T> res = mulPolyKaratsuba(move(a), move(b)); T carry = T();
  res.reserve(res.size() + 1); for (int i = 0; i < int(res.size()); i++) {
    T cur = res[i] + carry; res[i] = T(cur % BASE); carry = T(cur / BASE);
  }
  if (carry > T()) res.push_back(carry);
  return res;
}
