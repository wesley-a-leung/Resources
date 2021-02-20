#pragma once
#include <bits/stdc++.h>
#include "BinaryExponentiation.h"
#include "ModularArithmetic.h"
using namespace std;

// Computes n!
// Template Arguments:
//   T: the type of n
// Function Arguments:
//   n: the value to find the factorial of, must be non negative
// Return Value: n!
// In practice has a very small constant
// Time Complexity: O(n)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T factorial(T n) {
  T ret = 1; for (T i = 2; i <= n; i++) ret *= i;
  return ret;
}

// Computes n! modulo mod
// Template Arguments:
//   T: the type of n and mod
// Function Arguments:
//   n: the value to find the factorial of, must be non negative
//   mod: the modulo, must be not less than 2
// Return Value: n! modulo mod
// In practice has a very small constant
// Time Complexity: O(n)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T factorialMod(T n, T mod) {
  T ret = 1; for (T i = 2; i <= n; i++) ret = mulMod(ret, i, mod);
  return ret;
}

// Computes n permute k (n! / (n - k)!)
// Template Arguments:
//   T: the type of n and k
// Function Arguments:
//   n: the total number of elements, must be non negative
//   k: the number of elements to permute, must be in the range [0, n]
// Return Value: n permute k 
// In practice has a very small constant
// Time Complexity: O(min(k, n - k))
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T permute(T n, T k) {
  T ret = 1; for (T i = 0; i < k; i++) ret = ret * (n - i);
  return ret;
}

// Computes n permute k (n! / (n - k)!) modulo mod
// Template Arguments:
//   T: the type of n, k, and mod
// Function Arguments:
//   n: the total number of elements, must be non negative
//   k: the number of elements to permute
//   mod: the modulo, must be not less than 2
// Return Value: n permute k modulo mod
// In practice has a very small constant
// Time Complexity: O(min(k, n - k))
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T permuteMod(T n, T k, T mod) {
  T ret = 1; for (T i = 0; i < k; i++) ret = mulMod(ret, n - i, mod);
  return ret;
}

// Computes n choose k (n! / (k! (n - k)!))
// Template Arguments:
//   T: the type of n and k
// Function Arguments:
//   n: the total number of elements, must be non negative
//   k: the number of elements to choose, must be in the range [0, n]
// Return Value: n choose k 
// In practice has a very small constant
// Time Complexity: O(min(k, n - k))
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T choose(T n, T k) {
  if (k > n - k) k = n - k;
  T ret = 1; for (T i = 0; i < k; i++) ret = ret * (n - i) / (i + 1);
  return ret;
}

// Computes n choose k (n! / (k! (n - k)!)) modulo a prime p
// Template Arguments:
//   T: the type of n, k, and p
// Function Arguments:
//   n: the total number of elements, must be non negative
//   k: the number of elements to choose, must be in the range [0, n]
//   p: the prime modulo, must be greater than n
// Return Value: n permute k modulo p
// In practice has a very small constant
// Time Complexity: O(min(k, n - k) + log(p))
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T chooseModPrime(T n, T k, T p) {
  if (k > n - k) k = n - k;
  T num = 1, den = 1; for (T i = 0; i < k; i++) {
    num = mulMod(num, n - i, p); den = mulMod(den, i + 1, p);
  }
  return divModPrime(num, den, p);
}

// Computes n multichoose k ((n + k - 1)! / (k! (n - 1)!))
// Template Arguments:
//   T: the type of n and k
// Function Arguments:
//   n: the total number of elements, must be positive
//   k: the number of elements to choose with repetitions, must be non negative
// Return Value: n choose k 
// In practice has a very small constant
// Time Complexity: O(min(k, n - 1))
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T multiChoose(T n, T k) { return choose(n + k - 1, k); }

// Computes  n multichoose k ((n + k - 1)! / (k! (n - 1)!)) modulo a prime p
// Template Arguments:
//   T: the type of n, k, and p
// Function Arguments:
//   n: the total number of elements, must be positive
//   k: the number of elements to choose with repetitions, must be non negative
//   p: the prime modulo
// Return Value: n permute k modulo p
// In practice has a very small constant
// Time Complexity: O(min(k, n - 1) + log(p))
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T multiChooseModPrime(T n, T k, T p) {
  return chooseModPrime(n + k - 1, k, p);
}

// Combinatorics struct to compute factorials, permutations, and combinations
// Template Arguments:
//   T: the type of the value of N!
// Constructor Arguments:
//   N: the maximum value to compute factorials of
// Functions:
//   factorial(n): returns n! for 0 <= n <= N
//   permute(n, k): returns n permute k (n! / (n - k)!) for 0 <= k <= n <= N
//   choose(n, k): returns n permute k (n! / (k! (n - k)!))
//     for 0 <= k <= n <= N
//   multiChoose(n, k): returns n permute k ((n + k - 1)! / (k! (n - 1)!))
//     for n >= 1, k >= 0, n + k - 1 <= N
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N)
//   factorial, permute, choose, multiChoose: O(1)
// Memory Complexity: O(N)
// Tested:
//   Fuzz Tested
template <class T> struct Combinatorics {
  vector<T> fact; Combinatorics(int N) : fact(N + 1, 1) {
    for (int i = 1; i <= N; i++) fact[i] = fact[i - 1] * i;
  }
  T factorial(int n) { return fact[n]; }
  T permute(int n, int k) { return fact[n] / fact[n - k]; }
  T choose(int n, int k) { return fact[n] / fact[k] / fact[n - k]; }
  T multiChoose(int n, int k) { return choose(n + k - 1, k); }
};

// Combinatorics struct to compute factorials, permutations, and combinations
//   modulo a prime P
// Template Arguments:
//   T: the type of the value of N! and P
// Constructor Arguments:
//   N: the maximum value to compute factorials of
//   P: the prime modulo P, must be greater than N
// Functions:
//   factorial(n): returns n! modulo P for 0 <= n <= N
//   invFactorial(n): returns multiplicative inverse of n! modulo P
//     for 0 <= n <= N
//   permute(n, k): returns n permute k (n! / (n - k)!) modulo P
//     for 0 <= k <= n <= N
//   choose(n, k): returns n permute k (n! / (k! (n - k)!)) modulo P
//     for 0 <= k <= n <= N
//   multiChoose(n, k): returns n permute k ((n + k - 1)! / (k! (n - 1)!))
//     modulo P for n >= 1, k >= 0, n + k - 1 <= N
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N + log P)
//   factorial, invFactorial, permute, choose, multiChoose: O(1)
// Memory Complexity: O(N)
// Tested:
//   Fuzz Tested
template <class T> struct CombinatoricsModPrime {
  vector<T> fact, invFact; T P;
  CombinatoricsModPrime(int N, T P) : fact(N + 1, 1), invFact(N + 1), P(P) {
    for (int i = 1; i <= N; i++) fact[i] = mulMod(fact[i - 1], T(i), P);
    invFact[N] = mulInvModPrime(fact[N], P); for (int i = N - 1; i >= 0; i--)
      invFact[i] = mulMod(invFact[i + 1], T(i + 1), P);
  }
  T factorial(int n) { return fact[n]; }
  T invFactorial(int n) { return invFact[n]; }
  T permute(int n, int k) { return mulMod(fact[n], invFact[n - k], P); }
  T choose(int n, int k) {
    return mulMod(mulMod(fact[n], invFact[k], P), invFact[n - k], P);
  }
  T multiChoose(int n, int k) { return choose(n + k - 1, k); }
};

// Computes a row of Pascal's triangle
// Template Arguments:
//   T: the type of the values in the row of Pascal's triangle
// Function Arguments:
//   N: the row of Pascal's triangle to compute
// Return Value: a vector of length N + 1 with the values in the Nth row of
//   Pascal's triangle
// In practice, has a very small constant
// Time Complexity: O(N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz Tested
template <class T> vector<T> pascalsRow(int N) {
  vector<T> C(N + 1, 1);
  for (int j = 1; j <= N; j++) C[j] = C[j - 1] * (N - j + 1) / j;
  return C;
}

// Computes a row of Pascal's triangle modulo a prime p
// Template Arguments:
//   T: the type of the values in the row of Pascal's triangle
// Function Arguments:
//   N: the row of Pascal's triangle to compute
//   p: the prime modulo, must be greater than N
// Return Value: a vector of length N + 1 with the values in the Nth row of
//   Pascal's triangle modulo a prime p
// In practice, has a small constant
// Time Complexity: O(N log N)
// Memory Complexity: O(N)
template <class T> vector<T> pascalsRowModPrime(int N, T p) {
  vector<T> C(N + 1, 1); for (int j = 1; j <= N; j++)
    C[j] = divModPrime(mulMod(C[j - 1], T(N - j + 1), p), T(j), p);
  return C;
}

// Computes Pascal's triangle
// Template Arguments:
//   T: the type of the values in Pascal's triangle
// Function Arguments:
//   N: the maximum row of Pascal's triangle to compute
// Return Value: a vector of vectors length N + 1 with the ith vector having
//   length i + 1 with the values in the ith row of Pascal's triangle
// In practice, has a very small constant
// Time Complexity: O(N^2)
// Memory Complexity: O(N^2)
// Tested:
//   Fuzz Tested
template <class T> vector<vector<T>> pascalsTriangle(int N) {
  vector<vector<T>> C(N + 1); for (int i = 0; i <= N; i++) {
    C[i] = vector<T>(i + 1, 1); for (int j = 1; j < i; j++)
      C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
  }
  return C;
}

// Computes Pascal's triangle modulo mod
// Template Arguments:
//   T: the type of the values in Pascal's triangle and mod
// Function Arguments:
//   N: the maximum row of Pascal's triangle to compute
//   mod: the modulo, must be not less than 2
// Return Value: a vector of vectors length N + 1 with the ith vector having
//   length i + 1 with the values in the ith row of Pascal's triangle
//   modulo mod
// In practice, has a very small constant
// Time Complexity: O(N^2)
// Memory Complexity: O(N^2)
// Tested:
//   Fuzz Tested
template <class T> vector<vector<T>> pascalsTriangleMod(int N, T mod) {
  vector<vector<T>> C(N + 1); for (int i = 0; i <= N; i++) {
    C[i] = vector<T>(i + 1, 1); for (int j = 1; j < i; j++)
      C[i][j] = addMod(C[i - 1][j - 1], C[i - 1][j], mod);
  }
  return C;
}

// Computes the sum from 1 to n inclusive
// Template Arguments:
//   T: the type of n
// Function Arguments:
//   n: the maximum bound, must be non negative
// Return Value: the sum from 1 to n inclusive
// In practice has a very small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T sumTo(T n) { return n * (n + 1) / 2; }

// Computes the sum from a to b inclusive
// Template Arguments:
//   T: the type of n
// Function Arguments:
//   a: the lower bound, must be non negative
//   b: the upper bound, must be non negative and not less than a
// Return Value: the sum from a to b inclusive
// In practice has a very small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T> T sumBetween(T a, T b) { return sumTo(b) - sumTo(a - 1); }

// Computes the nth term of an arithmetic sequence with starting value a1 and
//   common difference d
// Template Arguments:
//   T: the type of a1 and d
//   U: the type of n
// Function Arguments:
//   a1: the starting value
//   d: the common difference
//   n: the index of the term to find, must be positive
// Return Value: the nth term of the arithmetic sequence
// In practice has a very small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T, class U>
T arithSeq(T a1, T d, U n) { return a1 + d * (n - 1); }

// Computes the sum of an arithmetic sequence with starting value a1 and
//   common difference d from term 1 to n
// Template Arguments:
//   T: the type of a1 and d
//   U: the type of n
// Function Arguments:
//   a1: the starting value
//   d: the common difference
//   n: the maximum index of the term to sum, must be positive
// Return Value: the sum of the arithmetic sequence form term 1 to n
// In practice has a very small constant
// Time Complexity: O(1)
// Memory Complexity: O(1)
//   Fuzz Tested
template <class T, class U>
T arithSeries(T a1, T d, U n) { return n * (a1 + arithSeq(a1, d, n)) / 2; }

// Computes the nth term of an geometric sequence with starting value a1 and
//   common ratio r
// Template Arguments:
//   T: the type of a1 and r
//   U: the type of n
// Function Arguments:
//   a1: the starting value
//   r: the common ratio
//   n: the index of the term to find, must be positive
// Return Value: the nth term of the geometric sequence
// In practice has a small constant
// Time Complexity: O(log n)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T, class U>
T geoSeq(T a1, T r, U n) { return a1 * pow2(r, n - 1); }

// Computes the sum of a geometric sequence with starting value a1 and
//   common ratio r from term 1 to n
// Template Arguments:
//   T: the type of a1 and r
//   U: the type of n
// Function Arguments:
//   a1: the starting value
//   r: the common ratio
//   n: the maximum index of the term to sum, must be positive
// Return Value: the sum of the geometric sequence form term 1 to n
// In practice has a small constant
// Time Complexity: O(log n)
// Memory Complexity: O(1)
// Tested:
//   Fuzz Tested
template <class T, class U> T geoSeries(T a1, T r, U n) {
  return r == 1 ? a1 * n : a1 * (T(1) - pow2(r, n)) / (T(1) - r);
}

// Computes the sum of floor((a * i + b) / m) for 0 <= i < n
// Equivalent to the number of integer points (x, y) where 0 <= x < n and
//   0 <= y <= (a * i + b) / m
// Template Arguments:
//   T: an integral type
// Function Arguments:
//   n: the upper bound for x
//   a: the linear factor's numerator
//   b: the constant factor's numerator
//   m: the common denominator
// Return Value: the sum of floor((a * i + b) / m) for 0 <= i < n
// In practice, has a small constant
// Time Complexity: O(log n)
// Memory Complexity: O(1)
// Tested:
//   https://judge.yosupo.jp/problem/sum_of_floor_of_linear
template <class T> T sumFloorLinear(T n, T a, T b, T m) {
  static_assert(is_integral<T>::value, "T must be integral");
  if (a == 0) return (b / m) * n;
  if (a >= m || b >= m) return ((a / m) * (n - 1) + 2 * (b / m)) * n / 2
                               + sumFloorLinear(n, a % m, b % m, m);
  return sumFloorLinear((a * n + b) / m, m, (a * n + b) % m, a);
}
