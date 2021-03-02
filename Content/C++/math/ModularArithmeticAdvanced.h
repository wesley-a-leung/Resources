#pragma once
#include <bits/stdc++.h>
#include "ModularArithmetic.h"
#include "Primes.h"
using namespace std;

// Computes the nth tetration of a modulo mod
// Assumes 0^0 = 1
// Template Arguments:
//   T: the type of base and mod
//   U: the type of the number of iterations
// Function Arguments:
//   a: the base
//   n: the number of iterations
//   mod: the modulo
// Return Value: the nth tetration of a modulo mod,
//   a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(sqrt(mod) log(mod))
// Memory Complexity: O(log(mod))
// Tested:
//   https://judge.yosupo.jp/problem/tetration_mod
template <class T, class U> T tetraMod(T a, U n, T mod) {
  if (mod == T(1)) return T(0);
  if (a == T(0)) return (T(1) - T(n % U(2))) % mod;
  if (n == U(0)) return T(1) % mod;
  if (n == U(1)) return a % mod;
  if (n == U(2)) return powMod(a % mod, a, mod);
  T p = phi(mod); return powMod(a % mod, tetraMod(a, n - 1, p) + p, mod);
}

// Helper function returning a * b % mod + mod if a * b >= mod, 
//   a * b % mod otherwise
template <class T> T mulMod2(T a, T b, T mod) {
  return (a *= b) >= mod ? a % mod + mod : a;
}

// Helper function returning base^pow % mod + mod if base^pow >= mod, 
//   base^pow % mod otherwise
template <class T, class U> T powMod2(T base, U pow, T mod) {
  if (base >= mod) base = base % mod + mod;
  T x = 1; while (true) {
    if (pow % 2 == 1) x = mulMod2(x, base, mod);
    if ((pow /= 2) == 0) break;
    base = mulMod2(base, base, mod);
  }
  return x;
}

// Helper function
template <class T> T powSeqModRec(const vector<T> &A, int i, T mod) {
  if (i == int(A.size()) - 1) return A[i];
  T p = phi(mod), res = powSeqModRec(A, i + 1, p);
  return res == T(0) ? T(1) : powMod2(A[i], res, mod);
}

// Computes A[0]^A[1]^...^A[N - 1] modulo mod
// Assumes 0^0 = 1
// Template Arguments:
//   T: the type of each element in the sequence
// Function Arguments:
//   A: the sequence
//   mod: the modulo
// Return Value: A[0]^A[1]^...^A[N - 1] modulo mod,
//   a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(N + sqrt(mod) log(mod))
// Memory Complexity: O(N + log(mod))
// Tested:
//   https://dmoj.ca/problem/dmopc20c3p5
template <class T> T powSeqMod(const vector<T> &A, T mod) {
  return powSeqModRec(A, 0, mod) % mod;
}

// Finds an integer x such that (x^2 = a) modulo a prime p
// If x is a solution, then (p - x) % p is the other solution
// Template Arguments:
//   T: the type of a and p
// Function Arguments:
//   a: the argument, must be in the range [0, p)
//   p: the prime modulo
// Return Value: the square root of a modulo p in the range [0, p), or -1 if
//   it does not exist,
// In practice, has a small constant
// Time Complexity: O(log(p)^2)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/sqrt_mod
template <class T> T sqrtMod(T a, T p) {
  if (a == T(0)) return 0;
  if (powMod(a, (p - 1) / 2, p) != T(1)) return -1;
  if (p % 4 == 3) return powMod(a, (p + 1) / 4, p);
  T s = p - 1, n = 2; int r = 0; for (; s % 2 == 0; s /= 2) r++;
  while (powMod(n % p, (p - 1) / 2, p) != p - 1) n++;
  T x = powMod(a, (s + 1) / 2, p), b = powMod(a, s, p);
  T g = powMod(n % p, s, p); while (true) {
    T t = b, m = T(0); for (; m < r && t != T(1); m++) t = mulMod(t, t, p);
    if (m == T(0)) return x;
    T gs = powMod(g, 1LL << (r - m - 1), p);
    g = mulMod(gs, gs, p); x = mulMod(x, gs, p); b = mulMod(b, g, p); r = m;
  }
  return -1;
}

// Finds the smallest non negative integer c such that
//   (a^c = b) modulo mod
// Assumes 0^0 = 1
// Template Arguments:
//   T: the type of a, b, and mod
// Function Arguments:
//   a: the base, must be in the range [0, mod)
//   b: the argument, must be in the range [0, mod)
//   mod: the modulo
// Return Value: the discrete log of b in base a modulo mod, or -1 if
//   it does not exist
// In practice, has a small constant
// Time Complexity: O(sqrt(mod) log(mod))
// Memory Complexity: O(sqrt(mod))
// Tested:
//   https://judge.yosupo.jp/problem/discrete_logarithm_mod
template <class T> T discreteLogMod(T a, T b, T mod) {
  if (b == T(1) % mod) return 0;
  T n = max(T(0), T(sqrt(mod)) - 1), e = T(1), f = T(1), j = T(1);
  vector<pair<T, T>> A; while (n * n <= mod) n++;
  while (j <= n && (e = f = mulMod(e, a, mod)) != b)
    A.emplace_back(mulMod(e, b, mod), j++);
  if (e == b) return j;
  sort(A.begin(), A.end()); if (gcd(mod, e) == gcd(mod, b)) {
    for (T i = 2; i <= n + 1; i++) {
      e = mulMod(e, f, mod);
      auto it = lower_bound(A.begin(), A.end(), make_pair(e, T(0)));
      if (it != A.end() && it->first == e) return n * i - it->second;
    }
  }
  return -1;
}
