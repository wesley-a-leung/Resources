#pragma once
#include <bits/stdc++.h>
using namespace std;

// Adds two nimbers a and b
// Template Arguments:
//   T: the type of a, b, must be an unsigned integral type
// Function Arguments:
//   a: the first value
//   b: the second value
// Return Value: a + b with nimber addition
// In practice, has a very small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/nim_product_64
template <class T> T addNim(T a, T b) {
  static_assert(is_unsigned<T>::value, "T must be an unsigned integral type");
  return a ^ b;
}

// Helper function to initialize the product cache array
template <class T, const int BITS> T initNim(T prod[BITS][BITS]) {
  for (int i = 0; i < BITS; i++) fill(prod[i], prod[i] + BITS, T());
  return T();
}

// Helper function to multiply 2^i and 2^j
template <class T, const int BITS>
T mulNimPow2(T prod[BITS][BITS], int i, int j) {
  static_assert(is_unsigned<T>::value, "T must be an unsigned integral type");
  T &res = prod[i][j]; if (res) return res;
  if (!(i & j)) return res = T(1) << (i | j);
  int a = (i & j) & -(i & j); return res = mulNimPow2(prod, i ^ a, j)
      ^ mulNimPow2(prod, (i ^ a) | (a - 1), (j ^ a) | (i & (a - 1)));
}

// Multiplies two nimbers a and b
// Template Arguments:
//   T: the type of a, b, must be an unsigned integral type
// Function Arguments:
//   a: the first value
//   b: the second value
// Return Value: a * b with nimber multiplication
// In practice, has a very small constant
// Time Complexity: O(BITS^2)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/nim_product_64
template <class T> T mulNim(T a, T b) {
  static_assert(is_unsigned<T>::value, "T must be an unsigned integral type");
  static constexpr const int BITS = 8 * sizeof(T);
  static T prod[BITS][BITS]; static T ZERO = initNim(prod);
  T res = ZERO; for (int i = 0; i < BITS; i++) if ((a >> i) & 1)
    for (int j = 0; j < BITS; j++) if ((b >> j) & 1)
      res = addNim(res, mulNimPow2(prod, i, j));
  return res;
}

// Computes base to the power pow in nimber arithmetic
// Template Arguments:
//   T: the type of base, must be an unsigned integral type
//   U: the type of pow
// Function Arguments:
//   base: the base
//   pow: the power, must be non negative and integral
// Return Value: base to the power pow in nimber arithmetic
// In practice, has a very small constant
// Time Complexity: O(BITS^2 log pow)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T, class U> T powNim(T base, U pow) {
  static_assert(is_unsigned<T>::value, "T must be an unsigned integral type");
  T x = 1; while (true) {
    if (pow % 2 == 1) x = mulNim(x, base);
    if ((pow /= 2) == 0) break;
    base = mulNim(base, base);
  }
  return x;
}

// Computes the multiplicative inverse of a in nimber arithmetic
// Guaranteed to exist
// Template Arguments:
//   T: the type of a, must be an unsigned integral type
// Function Arguments:
//   a: the value to find the inverse of
// Return Value: the multiplicative inverse of a in nimber arithmetic
// In practice, has a small constant
// Time Complexity: O(BITS^3)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T mulInvNim(T a) {
  static_assert(is_unsigned<T>::value, "T must be an unsigned integral type");
  return powNim(a, T(-2));
}
