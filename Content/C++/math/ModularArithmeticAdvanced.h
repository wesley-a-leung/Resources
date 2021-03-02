#pragma once
#include <bits/stdc++.h>
#include "ModularArithmetic.h"
#include "Primes.h"
using namespace std;

// Computes the nth tetration of a modulo mod
// Template Arguments:
//   T: the type of base
//   U: the type of number of iterations
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
template <class T> T tetraMod(T a, T n, T mod) {
  if (mod == T(1)) return T(0);
  if (a == T(0)) return (T(1) - n % T(2)) % mod;
  if (n == T(0)) return T(1) % mod;
  if (n == T(1)) return a % mod;
  if (n == T(2)) return powMod(a % mod, a, mod);
  T p = phi(mod); return powMod(a % mod, tetraMod(a, n - 1, p) + p, mod);
}

// Helper function returning a * b % mod + mod if a * b >= mod, 
//   a * b % mod otherwise
template <class T> T mulMod2(T a, T b, T mod) {
  return (a *= b) >= mod ? a % mod + mod : a;
}

// Helper function returning base ^ pow % mod + mod if base ^ pow >= mod, 
//   base ^ pow % mod otherwise
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

// Computes A[0] ^ A[1] ^ ... ^ A[N - 1] modulo mod
// Template Arguments:
//   T: the type of each element in the sequence
// Function Arguments:
//   A: the sequence
//   mod: the modulo
// Return Value: A[0] ^ A[1] ^ ... ^ A[N - 1] modulo mod,
//   a value in the range [0, mod)
// In practice, has a small constant
// Time Complexity: O(N + sqrt(mod) log(mod))
// Memory Complexity: O(N + log(mod))
// Tested:
//   https://dmoj.ca/problem/dmopc20c3p5
template <class T> T powSeqMod(const vector<T> &A, T mod) {
  return powSeqModRec(A, 0, mod) % mod;
}
