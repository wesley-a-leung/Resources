#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fast Fourier Transform
// Template Arguments:
//   F: the floating point type of the complex number components
// Function Arguments:
//   a: a reference to the vector of complex numbers (of type complex<F>)
//     to convolute
// In practice, has a moderate constant, faster than using Karatsuba, slower
//   than NTT
// Time Complexity: O(n log n) where n = size(a)
// Memory Complexity: O(n)
// Tested:
//   https://open.kattis.com/problems/polymul2
//   https://dmoj.ca/problem/atimesb
template <class F> void fft(vector<complex<F>> &a) {
  static_assert(is_floating_point<F>::value,
                "F must be a floating point type");
  static vector<complex<F>> rt(2, 1); static vector<int> ord; static int k = 2;
  static F PI = acos(F(-1)); int N = a.size(); assert(!(N & (N - 1)));
  for (; k < N; k <<= 1) {
    rt.resize(N); complex<F> x = polar(F(1), PI / k);
    for (int i = k; i < (k << 1); i++)
      rt[i] = i & 1 ? rt[i >> 1] * x : rt[i >> 1];
  }
  if (int(ord.size()) != N) {
    ord.assign(N, 0); int len = __builtin_ctz(N); for (int i = 0; i < N; i++)
      ord[i] = (ord[i >> 1] >> 1) + ((i & 1) << (len - 1));
  }
  for (int i = 0; i < N; i++) if (i < ord[i]) swap(a[i], a[ord[i]]);
  for (int len = 1; len < N; len <<= 1) for (int i = 0; i < N; i += len << 1)
    for (int j = 0; j < len; j++) {
      complex<F> u = a[i + j], x = a[len + i + j], y = rt[len + j];
      complex<F> v(real(x) * real(y) - imag(x) * imag(y),
                   real(x) * imag(y) + imag(x) * real(y));
      a[i + j] = u + v; a[len + i + j] = u - v;
    }
}

// Polynomial Multiplication
// Template Arguments:
//   T: the type of each element
//   F: the floating point type to use for FFT
// Functions Arguments:
//   a: a vector of type T representing the first polynomial, a[i] stores
//     the coefficient of x^i
//   b: a vector of type T representing the second polynomial, b[i] stores
//     the coefficient of x^i
// Return Value: a vector of type T representing the polynomial a times b with
//   no trailing zeros
// In practice, has a moderate constant
// Time Complexity: O(n log n) where n = size(a) + size(b)
// Memory Complexity: O(n)
// Tested:
//   https://open.kattis.com/problems/polymul2
const int FFT_CUTOFF = 30000;
template <class T, class F = long double>
vector<T> mulPolyFFT(const vector<T> &a, const vector<T> &b) {
  int N = max(0, int(a.size()) + int(b.size()) - 1);
  if ((long long)(a.size()) * (long long)(a.size()) <= FFT_CUTOFF) {
    vector<T> res(N, T()); for (int i = 0; i < int(a.size()); i++)
      for (int j = 0; j < int(b.size()); j++) res[i + j] += a[i] * b[j];
    while (int(res.size()) > 1 && res.back() == T()) res.pop_back();
    return res;
  }
  while (N & (N - 1)) N++;
  vector<complex<F>> f(N, complex<F>(0, 0));
  for (int i = 0; i < int(a.size()); i++) f[i].real(a[i]);
  for (int i = 0; i < int(b.size()); i++) f[i].imag(b[i]);
  fft(f); complex<F> r(0, F(-0.25) / N); for (int i = 0; i <= N / 2; i++) {
    int j = (N - i) & (N - 1);
    complex<F> prod = (f[j] * f[j] - conj(f[i] * f[i])) * r;
    f[i] = prod; f[j] = conj(prod);
  }
  fft(f); vector<T> res(N, T()); bool isIntegral = is_integral<T>::value;
  for (int i = 0; i < N; i++)
    res[i] = isIntegral ? round(real(f[i])) : real(f[i]);
  while (int(res.size()) > 1 && res.back() == T()) res.pop_back();
  return res;
}

// Integer Multiplication
// Template Arguments:
//   T: the type of each digit
//   F: the floating point type to use for FFT
// Functions Arguments:
//   a: a vector of type T representing the first integer, a[i] stores
//     the digit of BASE^i
//   b: a vector of type T representing the second polynomial, b[i] stores
//     the digit of BASE^i
//   BASE: the base of the integer
// Return Value: a vector of type T representing the integer a times b with
//   no trailing zeros
// In practice, has a moderate constant
// Time Complexity: O(n log n) where n = size(a) + size(b)
// Memory Complexity: O(n)
// Tested:
//   https://dmoj.ca/problem/atimesb
template <class T, class F = long double>
vector<T> mulIntFFT(const vector<T> &a, const vector<T> &b, T BASE = T(10)) {
  vector<T> res = mulPolyFFT<T, F>(a, b); T carry = T();
  res.reserve(res.size() + 1); for (int i = 0; i < int(res.size()); i++) {
    T cur = res[i] + carry; res[i] = T(cur % BASE); carry = T(cur / BASE);
  }
  if (carry > T()) res.push_back(carry);
  return res;
}
