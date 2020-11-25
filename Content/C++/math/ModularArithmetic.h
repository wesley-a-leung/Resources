#pragma once
#include <bits/stdc++.h>
#include "GCD.h"
using namespace std;

// Computes the positive modulo of a by mod
// Template Arguments:
//   T: the type of a and mod
// Function Arguments:
//   a: the value to find the positive modulo
//   mod: the modulo
// Return Value: the positive modulo of a by mod
// In practice, has a small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://dmoj.ca/problem/bts18p5
template <class T> T posMod(T a, T mod) {
  T ret = -mod < a && a < mod ? a : a % mod; return 0 <= ret ? ret : ret + mod;
}

// Adds two integers a and b modulo mod
// Template Arguments:
//   T: the type of a, b, and mod
// Function Arguments:
//   a: the first value, must be in the range [0, mod)
//   b: the second value, must be in the range [0, mod)
//   mod: the modulo
// Return Value: a + b modulo mod, a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/modulararithmetic
//   https://dmoj.ca/problem/bts18p5
template <class T> T addMod(T a, T b, T mod) {
  T ret = a + b; return ret < mod ? ret : ret - mod;
}

// Subtracts two integers a and b modulo mod
// Template Arguments:
//   T: the type of a, b, and mod
// Function Arguments:
//   a: the first value, must be in the range [0, mod)
//   b: the second value, must be in the range [0, mod)
//   mod: the modulo
// Return Value: a - b modulo mod, a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/modulararithmetic
template <class T> T subMod(T a, T b, T mod) {
  return a >= b ? a - b : a - b + mod;
}

// Multiplies two integers a and b modulo mod where a * b does not overflow
// Template Arguments:
//   T: the type of a, b, and mod
// Function Arguments:
//   a: the first value, must be in the range [0, mod)
//   b: the second value, must be in the range [0, mod)
//   mod: the modulo
// Return Value: a * b modulo mod, a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/modulararithmetic
//   https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
//   https://dmoj.ca/problem/bts18p5
template <class T> T mulMod(T a, T b, T mod) { return a * b % mod; }

// Computes base to the power pow modulo mod where mod * mod does not overflow
// Template Arguments:
//   T: the type of base and mod
//   U: the type of pow
// Function Arguments:
//   base: the base, must be in the range [0, mod)
//   pow: the power, must be non negative and integral
//   mod: the modulo
// Return Value: base to the power pow modulo mod,
//   a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(log pow)
// Memory Complexity: O(1)
// Tested:
//   https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
template <class T, class U> T powMod(T base, U pow, T mod) {
  T x = 1; while (true) {
    if (pow % 2 == 1) x = mulMod(x, base, mod);
    if ((pow /= 2) == 0) break;
    base = mulMod(base, base, mod);
  }
  return x;
}

// Computes the multiplicative inverse of a for a prime mod p where p * p
//   does not overflow
// Guaranteed to exist
// Template Arguments:
//   T: the type of a and p
// Function Arguments:
//   a: the value to find the inverse of, must be in the range [0, p)
//   p: the prime mod
// Return Value: the multiplicative inverse of a for a prime mod p
// In practice, has a small constant
// Time Complexity: O(log p)
// Memory Complexity: O(1)
// Tested:
//   https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
template <class T> T mulInvModPrime(T a, T p) { return powMod(a, p - 2, p); }

// Divides a by b modulo a prime p where p * p does not overflow
// Guaranteed to be valid
// Template Arguments:
//   T: the type of a, b and p
// Function Arguments:
//   a: the dividend, must be in the range [0, p)
//   b: the divisor, must be in the range [1, p)
//   p: the prime mod
// Return Value: a / b modulo p
// In practice, has a small constant
// Time Complexity: O(log p)
// Memory Complexity: O(1)
// Tested:
//   https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
template <class T> T divModPrime(T a, T b, T p) {
  return mulMod(a, mulInvModPrime(b, p), p);
}

// Divides a by b modulo mod
// Only valid if gcd(b, mod) == 1
// Template Arguments:
//   T: the type of a, b and mod
// Function Arguments:
//   a: the dividend, must be in the range [0, mod)
//   b: the divisor, must be in the range [1, mod)
//   mod: the modulo
// Return Value: a / b modulo mod, or -1 if invalid
// In practice, has a small constant
// Time Complexity: O(log p)
// Memory Complexity: O(1)
// Tested:
//   https://open.kattis.com/problems/modulararithmetic
template <class T> T divMod(T a, T b, T mod) {
  T inv = mulInv(b, mod); return inv == -1 ? -1 : mulMod(a, inv, mod);
}

// The following functions are useful when mod * mod overflow

// Multiplies two integers a and b modulo mod where a * b does overflow, but
//   mod + mod does not overflow
// Template Arguments:
//   T: the type of a, b, and mod
// Function Arguments:
//   a: the first value, must be in the range [0, mod)
//   b: the second value, must be in the range [0, mod)
//   mod: the modulo
// Return Value: a * b modulo mod, a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(log b)
// Memory Complexity: O(1)
// Tested:
//   https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
template <class T> T mulModOvf(T a, T b, T mod) {
  T x = 0; while (true) {
    if (b % 2 == 1) x = addMod(x, a, mod);
    if ((b /= 2) == 0) break;
    a = addMod(a, a, mod);
  }
  return x;
}

// Computes base to the power pow modulo mod where mod * mod overflows, but
//   mod + mod does not overflow
// Template Arguments:
//   T: the type of base and mod
//   U: the type of pow
// Function Arguments:
//   base: the base, must be in the range [0, mod)
//   pow: the power, must be non negative and integral
//   mod: the modulo
// Return Value: base to the power pow modulo mod,
//   a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(log pow log mod)
// Memory Complexity: O(1)
// Tested:
//   https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
template <class T, class U> T powModOvf(T base, U pow, T mod) {
  T x = 1; while (true) {
    if (pow % 2 == 1) x = mulModOvf(x, base, mod);
    if ((pow /= 2) == 0) break;
    base = mulModOvf(base, base, mod);
  }
  return x;
}

// Computes the multiplicative inverse of a for a prime mod p where p * p
//   overflows, but mod + mod does not overflow
// Guaranteed to exist
// Template Arguments:
//   T: the type of a and p
// Function Arguments:
//   a: the value to find the inverse of, must be in the range [0, p)
//   p: the prime mod
// Return Value: the multiplicative inverse of a for a prime mod p
// In practice, has a small constant
// Time Complexity: O(log p log mod)
// Memory Complexity: O(1)
// Tested:
//   https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
template <class T> T mulInvModPrimeOvf(T a, T p) {
  return powModOvf(a, p - 2, p);
}

// Divides a by b modulo a prime p where p * p overflows, but mod + mod does
//   not overflow
// Guaranteed to be valid
// Template Arguments:
//   T: the type of a, b and p
// Function Arguments:
//   a: the dividend, must be in the range [0, p)
//   b: the divisor, must be in the range [1, p)
//   p: the prime mod
// Return Value: a / b modulo p
// In practice, has a small constant
// Time Complexity: O(log p log mod)
// Memory Complexity: O(1)
// Tested:
//   https://atcoder.jp/contests/m-solutions2019/tasks/m_solutions2019_e
template <class T> T divModPrimeOvf(T a, T b, T p) {
  return mulModOvf(a, mulInvModPrimeOvf(b, p), p);
}

// Struct supporting operations in Montgomery form
// Constructor Arguments:
//   mod: the modulo of the space
// Functions:
//   init(x): transforms a number into Montgomery form
//   reduce(x): transforms a number from Montgomery form
//   mul(a, b): multiplies the numbers a and b in Montgomery form and returns
//     their product modulo mod in Montgomery form
// In practice, has a small constant
// Time Complexity:
//   constructor. init, reduce, mul: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://loj.ac/p/6466
//   https://www.spoj.com/problems/FACT2/
struct Montgomery {
  using u64 = uint64_t; using u128 = __uint128_t; using s128 = __int128_t;
  struct u256 {
    static u128 HI(u128 x) { return x >> 64; }
    static u128 LO(u128 x) { return u64(x); }
    u128 hi, lo; u256(u128 lo = 0) : hi(0), lo(lo) {}
    u256(u128 hi, u128 lo) : hi(hi), lo(lo) {}
    static u256 mul(u128 x, u128 y) {
      u128 t1 = LO(x) * LO(y), t2 = HI(x) * LO(y) + HI(y) * LO(x) + HI(t1);
      return u256(HI(x) * HI(y) + HI(t2), (t2 << 64) + LO(t1));
    }
  };
  u128 mod, inv, r2;
  Montgomery(u128 mod = 1) : mod(mod), inv(1), r2(-mod % mod) {
    for (int i = 0; i < 7; i++) inv *= 2 - mod * inv;
    for (int i = 0; i < 4; i++) if ((r2 <<= 1) >= mod) r2 -= mod;
    for (int i = 0; i < 5; i++) r2 = mul(r2, r2);
  }
  u128 init(u128 x) { return mul(x, r2); }
  u128 reduce(u256 x) {
    u128 q = x.lo * inv; s128 a = x.hi - u256::mul(q, mod).hi;
    return a < 0 ? a + mod : a;
  }
  u128 mul(u128 a, u128 b) { return reduce(u256::mul(a, b)); }
};

// Multiplies two unsigned 128-bit integers a and b modulo mod even if a * b
//   overflows, using Montgomery reduction
// Function Arguments:
//   a: the first value, must be in the range [0, mod)
//   b: the second value, must be in the range [0, mod)
//   mod: the modulo
// Return Value: a * b modulo mod, a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   https://loj.ac/p/6466
//   https://www.spoj.com/problems/FACT2/
__uint128_t mulMod(__uint128_t a, __uint128_t b, __uint128_t mod) {
  static Montgomery mont; if (mont.mod != mod) mont = Montgomery(mod);
  return mont.reduce(mont.mul(mont.init(a), mont.init(b)));
}
