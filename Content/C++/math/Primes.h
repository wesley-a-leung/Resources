#pragma once
#include <bits/stdc++.h>
#include "ModularArithmetic.h"
#include "GCD.h"
#include "../utils/Random.h"
using namespace std;

// constexpr function (in C++14 and above) to determine if x is prime
// Template Arguments:
//   T: the type of x
// Function Arguments:
//   x: the value to check if prime
// Return Value: true if x is prime, false otherwise
// In practice, has a small constant
// Time Complexity: O(sqrt x)
// Memory Complexity: O(1)
// Tested:
//   https://www.spoj.com/SHORTEN/problems/ISPRIME/
//   https://dmoj.ca/problem/angieandfunctions
template <class T> constexpr bool isPrime(T x) {
  if (x < 2) return false;
  for (T i = 2; i * i <= x; i++) if (x % i == 0) return false;
  return true;
}

// Finds the prime factors of x
// Template Arguments:
//   T: the type of x
// Function Arguments:
//   x: the value to prime factor
// Return Value: a sorted vector of T with the prime factorization of x
// In practice, has a small constant
// Time Complexity: O(sqrt x)
// Memory Complexity: O(log x)
// Tested:
//   https://www.spoj.com/problems/FACT0/
template <class T> vector<T> primeFactor(T x) {
  vector<T> ret; for (T i = 2; i * i <= x; i++)
    while (x % i == 0) { ret.push_back(i); x /= i; }
  if (x > 1) ret.push_back(x);
  return ret;
}

// Finds the prime factors of x and its exponent
// Template Arguments:
//   T: the type of x
// Function Arguments:
//   x: the value to prime factor
// Return Value: a sorted vector of pairs of T and int with each
//   prime factor of x and its exponent
// In practice, has a small constant
// Time Complexity: O(sqrt x)
// Memory Complexity: O(log x)
// Tested:
//   https://www.spoj.com/problems/FACT0/
template <class T> vector<pair<T, int>> primeFactorWithCount(T x) {
  vector<pair<T, int>> ret; for (T i = 2; i * i <= x; i++) if (x % i == 0)
    for (ret.emplace_back(i, 0); x % i == 0; x /= i) ret.back().second++;
  if (x > 1) ret.emplace_back(x, 1);
  return ret;
}

// Finds the factors of x
// Template Arguments:
//   T: the type of x
// Function Arguments:
//   x: the value to find the factors
// Return Value: a vector of T with the factors of x,
//   not guaranteed to be sorted
// In practice, has a small constant
// Time Complexity: O(sqrt x)
// Memory Complexity: O(sqrt x)
// Tested:
//   https://www.spoj.com/problems/CZ_PROB2/
template <class T> vector<T> factor(T x) {
  vector<T> ret; for (T i = 1; i * i <= x; i++)
    if (x % i == 0) { ret.push_back(i); if (x / i != i) ret.push_back(x / i); }
  return ret;
}

// Determine if x is prime using the Miller Rabin Primality Test
// Template Arguments:
//   T: the type of x
// Function Arguments:
//   x: the value to check if prime, mulMod(x, x, x) should not overflow
//   iterations: number of iterations to run, should be at least 7
// Return Value: true if x is prime, false otherwise, can return
//   false positives for x > 7e18, otherwise is guaranteed to be correct
// In practice, has a small constant
// Time Complexity: O(time complexity of powMod)
// Memory Complexity: O(iterations)
// Tested:
//   https://dmoj.ca/problem/bf3hard
template <class T> bool millerRabin(T x, int iterations = 7) {
  if (x < 2 || x % 6 % 4 != 1) return (x | 1) == 3;
  vector<T> A = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
  while (int(A.size()) < iterations)
    A.push_back(uniform_int_distribution<long long>(
        1795265023, numeric_limits<long long>::max())(rng64));
  int s = 0; while (!(((x - 1) >> s) & 1)) s++;
  T d = x >> s; for (T a : A) {
    T p = powMod(a % x, d, x); int i = s;
    while (p != 1 && p != x - 1 && a % x != 0 && i--) p = mulMod(p, p, x);
    if (p != x - 1 && i != s) return false;
  }
  return true;
}

// Sieve of Erathosthenes to identify primes less than or equal to N
// Constructor Arguments:
//   N: the maximum value
//   f(i): the function to run a callback on for each prime i
// Fields:
//   p: a vector of booleans with the ith indicating whether the integer
//     i * 2 + 1 is prime or not
// Functions:
//   isPrime(x): returns whether x is prime or not
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N log log N)
//   isPrime: O(1)
// Memory Complexity: O(N / 128)
// Tested:
//   https://judge.yosupo.jp/problem/enumerate_primes
//   https://codeforces.com/contest/1149/problem/A
struct Sieve {
  vector<bool> p;
  template <class F = function<void(int)>>
  Sieve(int N, F f = [] (int) {}) : p(N / 2 + 1, true) {
    p[0] = false; if (N >= 2) f(2);
    for (int i = 3; i <= N; i += 2) if (p[i / 2]) {
      f(i); if (i <= (N + i - 1) / i) for (int j = i * i; j <= N; j += i * 2)
        p[j / 2] = false;
    }
  }
  bool isPrime(int x) { return x == 2 || (x % 2 == 1 && p[x / 2]); }
};

// Sieve of Erathosthenes to identify primes and the smallest prime factor of
//   each number from 1 to N
// Constructor Arguments:
//   N: the maximum value
// Fields:
//   isPrime: a vector of N + 1 booleans inidicating whether each integer is
//     prime or not
//   SPF: a vector of N + 1 integers representing the smallest prime factor
//     less than or equal to each integer
//   primes: a vector of all primes less than or equal to N
// Functions:
//   primeFactor(x): returns a sorted vector of integers with the
//     prime factorization of x
//   primeFactorWithCount(x): returns a sorted vector of pairs of integers
//     with each prime factor of x and its exponent
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   primeFactor, primeFactorWithCount: O(log x)
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/1034/problem/A
//   https://tlx.toki.id/problems/troc-16/E
struct SievePrimeFactoring {
  vector<bool> isPrime; vector<int> SPF, primes;
  SievePrimeFactoring(int N) : isPrime(N + 1, true), SPF(N + 1) {
    isPrime[0] = isPrime[1] = false; for (int i = 2; i <= N; i++) {
      if (isPrime[i]) primes.push_back(SPF[i] = i);
      for (int j = 0; j < int(primes.size())
          && i * primes[j] <= N && primes[j] <= SPF[i]; j++) {
        isPrime[i * primes[j]] = false; SPF[i * primes[j]] = primes[j];
      }
    }
  }
  vector<int> primeFactor(int x) {
    vector<int> ret; while (x > 1) { ret.push_back(SPF[x]); x /= SPF[x]; }
    return ret;
  }
  vector<pair<int, int>> primeFactorWithCount(int x) {
    vector<pair<int, int>> ret; while (x > 1) {
      ret.emplace_back(SPF[x], 0);
      for (int spf = SPF[x]; x % spf == 0; x /= spf) ret.back().second++;
    }
    return ret;
  }
};

// Computes phi(x) which is the number of positive integers less than or equal
//   to x that are relatively prime to x
// Template Arguments:
//   T: the type of x
// Function Arguments:
//   x: the value to find phi(x)
// Return Value: phi(x)
// In practice, has a small constant
// Time Complexity: O(sqrt x)
// Memory Complexity: O(1)
// Tested:
//   https://www.spoj.com/problems/ETF/
template <class T> T phi(T x) {
  T ret = x; for (T i = 2; i * i <= x; i++) if (x % i == 0)
    for (ret -= ret / i; x % i == 0; x /= i);
  if (x > 1) ret -= ret / x;
  return ret;
}

// Euler's Totient function
// Constructor Arguments:
//   N: the maximum value
// Fields:
//   phi: a vector of integers with phi[i] equal to the number of positive
//     integers less than or equal to i that are relatively prime to i
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N log log N)
// Memory Complexity: O(N)
// Tested:
//   https://www.spoj.com/problems/ETF/
struct EulersTotient {
  vector<int> phi;
  EulersTotient(int N) : phi(N + 1) {
    iota(phi.begin(), phi.end(), 0);
    for (int i = 2; i <= N; i++) if (phi[i] == i) {
      phi[i] = i - 1;
      for (int j = i * 2; j <= N; j += i) phi[j] = phi[j] / i * (i - 1);
    }
  }
};

// Segmented Sieve of Erathosthenes to identify primes between lo and hi
// Template Arguments:
//   T: the type of lo and hi
//   F: the type of f
// Functions Arguments:
//   lo: the inclusive lower bound
//   hi: the inclusive upper bound
//   f(i): the function to run a callback on for each prime i
// In practice, has a small constant
// Time Complexity: O(sqrt(hi) log(log(hi)) + (hi - lo))
// Memory Complexity: O(sqrt(hi) + (hi - lo))
// Tested:
//   https://dmoj.ca/problem/phantom3
template <class T, class F> void segmentedSieve(T lo, T hi, F f) {
  lo = max(lo, T(2)); T sqrtHi = sqrtl(hi) + 2;
  while (sqrtHi * sqrtHi > hi) sqrtHi--;
  vector<bool> p1(sqrtHi + 1, false), p2(hi - lo + 1, false);
  p1[0] = p1[1] = true; for (T i = 2; i <= sqrtHi; i++) if (!p1[i]) {
    for (T j = i * i; j <= sqrtHi; j += i) p1[j] = true;
    for (T j = (lo + i - 1) / i * i; j <= hi; j += i)
      if (j != i && !p2[j - lo]) p2[j - lo] = true;
  }
  for (T i = 0; i < hi - lo + 1; i++) if (!p2[i]) f(lo + i);
}

// Returns an arbitrary divisor of N
// Template Arguments:
//   T: the type of N
// Function Arguments:
//   N: the value to find a divisor, mulMod(N, N, N) should not overflow
//   iterations: number of iterations to run, should be at least 40
// Return Value: an arbitrary divisor of N
// In practice, has a moderate constant
// Time Complexity: O(N^(1/4))
// Memory Complexity: O(1)
// Tested:
//   https://loj.ac/p/6466
//   https://www.spoj.com/problems/FACT2/
//   https://judge.yosupo.jp/problem/factorize
template <class T> T pollardsRho(T N, int iterations = 40) {
  if (N == 1) return 1;
  auto f = [&] (T x) { return mulMod(x, x, N) + 1; };
  T x = 0, y = 0, p = 2, q; int t = 0, i = 1;
  while (t++ % iterations != 0 || gcd(p, N) == 1) {
    if (x == y) y = f(x = ++i);
    if ((q = mulMod(p, max(x, y) - min(x, y), N)) != 0) p = q;
    x = f(x); y = f(f(y));
  }
  return gcd(p, N);
}

// Prime factors a large integer x
// Template Arguments:
//   T: the type of N
// Function Arguments:
//   x: the value to prime factor, mulMod(x, x, x) should not overflow
//   pollardsRhoIters: number of iterations to run pollard rho, should be
//     at least 40
//   millerRabinIters: number of iterations to run miller rabin, should be
//     at least 7
// Return Value: a sorted vector of T with the prime factorization of x
// In practice, has a moderate constant
// Time Complexity: O(x^(1/4) log(x))
// Memory Complexity: O(log x)
// Tested:
//   https://www.spoj.com/problems/FACT2/
//   https://judge.yosupo.jp/problem/factorize
template <class T>
vector<T> pollardsRhoPrimeFactor(T x, int pollardsRhoIters = 40,
                                 int millerRabinIters = 7) {
  if (x == 1) return vector<T>();
  vector<T> ret; queue<T> q; q.push(x); while (!q.empty()) {
    T y = q.front(); q.pop();
    if (millerRabin(y, millerRabinIters)) ret.push_back(y);
    else { q.push(pollardsRho(y, pollardsRhoIters)); q.push(y / q.back()); }
  }
  return ret;
}

// Prime factors a large integer x and its exponent
// Template Arguments:
//   T: the type of N
// Function Arguments:
//   x: the value to prime factor, mulMod(x, x, x) should not overflow
//   pollardsRhoIters: number of iterations to run pollard rho, should be
//     at least 40
//   millerRabinIters: number of iterations to run miller rabin, should be
//     at least 7
// Return Value: a sorted vector of pairs of T and int with each
//   prime factor of x and its exponent
// In practice, has a moderate constant
// Time Complexity: O(x^(1/4) log(x))
// Memory Complexity: O(log x)
// Tested:
//   https://www.spoj.com/problems/FACT2/
template <class T>
vector<pair<T, int>> pollardsRhoPrimeFactorWithCount(
    T x, int pollardsRhoIters = 40, int millerRabinIters = 7) {
  vector<T> pf = pollardsRhoPrimeFactor(x, pollardsRhoIters, millerRabinIters);
  sort(pf.begin(), pf.end()); vector<pair<T, int>> ret;
  for (int i = 0, cnt = 0; i < int(pf.size()); i++) {
    cnt++; if (i + 1 == int(pf.size()) || pf[i] != pf[i + 1]) {
      ret.emplace_back(pf[i], cnt); cnt = 0;
    }
  }
  return ret;
}

// Determines the factors of all numbers from 1 to N
// Constructor Arguments:
//   N: the maximum value
// Fields:
//   factors: a vector of vector of integers with factors[i] being the sorted
//     factors of i
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N log N)
// Memory Complexity: O(N log N)
// Tested:
//   https://codeforces.com/contest/111/problem/B
struct Factors {
  vector<vector<int>> factors;
  Factors(int N) : factors(N + 1) {
    for (int i = 1; i <= N; i++) for (int j = i; j <= N; j += i)
      factors[j].push_back(i);
  }
};
