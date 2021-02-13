#pragma once
#include <bits/stdc++.h>
#include "BinaryExponentiation.h"
#include "Primes.h"
using namespace std;

// Struct representing integers modulo MOD
// Template Arguments:
//   T: the type of the integer, must be integral
//   MOD: the value to mod by, must be non negative
//   PRIME_MOD: a boolean indicating whether MOD is prime, can be computed
//     automatically if C++ 14 or above, and if MOD is small
//   MUL_OVERFLOW: a boolean indicating whether MOD * MOD overflows, can be
//     computed automatically if numeric_limits<T>::max() exists
// Constructor Arguments:
//   x: the value to initialize the struct with
// Fields:
//   v: the value
// Functions:
//   <, <=, >, >=, ==, !=: comparison operators
//   ++, --, +, +=, -, -=, *, *=: standard arithmetic operators modulo MOD
//   pow(p): returns this value raises this to the power of p
//   mulInv(): returns the multiplicative inverse of this value, only exists
//     if gcd(v, MOD) is 1
//   /, /=: division in modular arithmetic, only valid if a multiplicative
//     inverse of the divisor exists
//   >>, <<: input and output operators
// Time Complexity:
//   constructor: O(1)
//   <, <=, >, >=, ==, !=, ++, --, +, +=, -, -=, >>, <<: O(1)
//   *, *=: O(1) if MUL_OVERFLOW is false, O(log MOD) otherwise
//   pow: O(log MOD) if MUL_OVERFLOW is false, O((log MOD)^2) otherwise
//   mulInv, /, /=: O(log MOD)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/angieandfunctions
template <class T, const T MOD,
#if __cplusplus < 201402L
          const bool PRIME_MOD,
#else
          const bool PRIME_MOD = isPrime(MOD),
#endif
          const bool MUL_OVERFLOW = (numeric_limits<T>::max() / MOD < MOD)>
    struct IntMod {
  static_assert(is_integral<T>::value, "T must be an integral type");
  static_assert(is_signed<T>::value, "T must be a signed type");
  static_assert(0 < MOD, "MOD must be a positive integer");
  using IM = IntMod<T, MOD, PRIME_MOD, MUL_OVERFLOW>;
  T v; IntMod() : v(0) {}
  IntMod(const T &x) {
    v = -MOD < x && x < MOD ? x : x % MOD; if (v < 0) v += MOD;
  }
  bool operator < (const IM &i) const { return v < i.v; }
  bool operator <= (const IM &i) const { return v <= i.v; }
  bool operator > (const IM &i) const { return v > i.v; }
  bool operator >= (const IM &i) const { return v >= i.v; }
  bool operator == (const IM &i) const { return v == i.v; }
  bool operator != (const IM &i) const { return v != i.v; }
  IM operator ++ () {
    if (++v == MOD) v = 0;
    return *this;
  }
  IM operator ++ (int) {
    IM ret = *this; if (++v == MOD) v = 0;
    return ret;
  }
  IM operator -- () {
    if (v-- == 0) v = MOD - 1;
    return *this;
  }
  IM operator -- (int) {
    IM ret = *this; if (v-- == 0) v = MOD - 1; return ret;
  }
  IM operator + (const IM &i) const { return IM(*this) += i; }
  IM &operator += (const IM &i) {
    if ((v += i.v) >= MOD) v -= MOD;
    return *this;
  }
  IM operator - (const IM &i) const { return IM(*this) -= i; }
  IM &operator -= (const IM &i) {
    if ((v -= i.v) < 0) v += MOD;
    return *this;
  }
  IM operator + () const { return *this; }
  IM operator - () const { return IM(-v); }
  IM operator * (const IM &i) const { return IM(*this) *= i; }
  IM &operator *= (const IM &i) {
    if (MUL_OVERFLOW) {
      IM x = 0, y = *this; T z = i.v;
      for (; z > 0; z /= 2, y += y) if (z % 2 == 1) x += y;
      *this = x;
    } else v = v * i.v % MOD;
    return *this;
  }
  IM mulInv() const {
    if (!PRIME_MOD || MUL_OVERFLOW) {
      T g = MOD, r = v, x = 0, y = 1; while (r != 0) {
        T q = g / r; g %= r; swap(g, r); x -= q * y; swap(x, y);
      }
      assert(g == 1); return IM(x);
    } else return pow2(*this, MOD - 2);
  }
  IM operator / (const IM &i) const { return IM(*this) /= i; }
  IM &operator /= (const IM &i) { return *this *= i.mulInv(); }
  friend istream &operator >> (istream &stream, IM &i) {
    T v; stream >> v; i = IM(v); return stream;
  }
  friend ostream &operator << (ostream &stream, const IM &i) {
    return stream << i.v;
  }
};
