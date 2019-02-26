#pragma once
#include <bits/stdc++.h>
using namespace std;

// n! % m
// Time Complexity: O(n)
template <class T> T factorial(T n, T m) {
    T ret = 1;
    for (int i = 2; i <= n; i++) ret = ret * i % m;
    return ret;
}

// n! % p for a prime p
// Time Complexity: O(p log n)
template <class T> T factorialPrime(T n, T p) {
    T ret = 1, h = 0;
    while (n > 1) {
        ret = (ret * ((n / p) % 2 == 1 ? p - 1 : 1)) % p; h = n % p; n /= p;
        for (int i = 2; i <= h; i++) ret = ret * i % p;
    }
    return ret;
}

// (a + b) % mod
// Time Complexity O(1)
// Required: 0 <= a < mod, 0 <= b < mod, mod + mod does not overflow
template <class T> addMod(T a, T b, T mod) { T ret = a + b; return ret < mod ? ret : ret - mod; }

// (a - b) % mod
// Time Complexity O(1)
// Required: 0 <= a < mod, 0 <= b < mod, mod can be represented as a signed integer
template <class T> subMod(T a, T b, T mod) { T ret = a - b; return 0 <= ret ? ret : ret + mod; }

// a * b % mod
// Time Complexity: O(1)
// Required: 0 <= a < mod, 0 <= b < mod, a * b does not overflow
template <class T> mulMod(T a, T b, T mod) { return a * b % mod; }

// a * b % mod, useful if a * b overflows
// Time Complexity: O(log b)
// Required: 0 <= a < mod, 0 <= b < mod, mod + mod does not overflow
template <class T> T mulMod2(T a, T b, T mod) {
    T x = 0, y = a;
    for (; b > 0; b /= 2, y = y + y < mod ? y + y : y + y - mod) if (b % 2 == 1) x = x + y < mod ? x + y : x + y - mod;
    return x;
}

// base ^ pow
// Time Complexity: O(log pow)
// If multiplication is an expensive operation, then y = y * y should only be computed when pow > 0
// Required: 0 <= pow
template <class T, class U> T pow2(T base, U pow) {
    T x = 1, y = base;
    for (; pow > 0; pow /= 2, y = y * y) if (pow % 2 == 1) x = x * y;
    return x;
}

// base ^ pow % mod
// Time Complexity: O(log pow)
// If multiplication is an expensive operation, then y = y * y should only be computed when pow > 0
// Required: 0 <= base < mod, 0 <= pow, 0 < mod, mod * mod does not overflow
template <class T, class U> T powMod(T base, U pow, T mod) {
    T x = 1, y = base;
    for (; pow > 0; pow /= 2, y = y * y % mod) if (pow % 2 == 1) x = x * y % mod;
    return x;
}

// Modular Multiplicative Inverse of i in Zp for a prime p
// Time Complexity: O(log p)
// Required: 0 < i < p, p * p does not overflow
template <class T> T mulInv(T i, T p) { return powMod(i, p - 2, p); }

// i / j % p for a prime p
// Time Complexity: O(log p)
// Required: 0 <= i < p, 0 < j < p, p * p does not overflow
template <class T> T divMod(T i, T j, T p) { return i * mulInv(j) % p; }

// n choose k
// Time Complexity: O(min(k, n - k))
template <class T> choose(int n, int k) {
    if (n < k) return 0;
    if (k > n - k) k = n - k;
    T ret = 1;
    for (int i = 0; i < k; i++) ret = ret * (n - i) / (i + 1);
    return ret;
}

// n choose k % p for a prime p
// Time Complexity: O(min(k, n - k))
template <class T> T choose(int n, int k, T p) {
    if (n < k) return 0;
    if (k > n - k) k = n - k;
    T num = 1, den = 1;
    for (int i = 0; i < k; i++) { num = (num * (n - i)) % p; den = (den * (i + 1)) % p; }
    return divMod(num, den, p);
}

// n choose k % p
// Time Complexity: O(log p) if factorials are precomputed
template <class T> T fastChoose(int n, int k, T p) { return divMod(divMod(factorial(n, p), factorial(k, p), p), factorial(n - k, p), p); }

// choosing k elements from n items with replacement, modulo p
// Time Complexity: O(log p) if factorials are precomputed
template <class T> T multiChoose(int n, int k, T p) { return fastChoose(n + k - 1, k, p); }

// n permute k
// Time Complexity: O(min(k, n - k))
template <class T> permute(int n, int k) {
    if (n < k) return 0;
    if (k > n - k) k = n - k;
    T ret = 1;
    for (int i = 0; i < k; i++) ret = ret * (n - i);
    return ret;
}

// n permute k % m
// Time Complexity: O(min(k, n - k))
template <class T> T permute(int n, int k, T m) {
    if (n < k) return 0;
    if (k > n - k) k = n - k;
    T ret = 1;
    for (int i = 0; i < k; i++) ret = ret * (n - i) % m;
    return ret;
}

// n permute k % p
// Time Complexity: O(log p) if factorials are precomputed
template <class T> T fastPermute(int n, int k, T p) { return divMod(factorial(n, p), factorial(n - k, p), p); }

// Structure to support combinatorical queries
// Time Complexity:
//   init (no modulo / modulo): O(N)
//   factorial, invFactorial, permute, choose, multiChoose: O(1)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct Combinatorics {
    T fact[MAXN], invFact[MAXN];
    void init(int N) { // compute factorials up to N!
        fact[0] = 1;
        for (int i = 1; i <= N; i++) fact[i] = fact[i - 1] * i;
    }
    void init(int N, T P) { // compute factorials mod prime up to N!
        assert(N < P); fact[0] = 1;
        for (int i = 1; i <= N; i++) fact[i] = fact[i - 1] * i % P;
        invFact[N] = mulInv(fact[N], P);
        for (int i = N - 1; i >= 0; i--) invFact[i] = invFact[i + 1] * (i + 1) % P;
    }
    T factorial(int N) { return fact[N]; }
    T invFactorial(int N) { return invFact[N]; }
    T permute(int N, int K) { return fact[N] / fact[N - K]; }
    T permute(int N, int K, T P) { return fact[N] * invFact[N - K] % P; }
    T choose(int N, int K) { return fact[N] / fact[K] / fact[N - K]; }
    T choose(int N, int K, T P) { return fact[N] * invFact[K] % P * invFact[N - K] % P; }
    T multiChoose(int N, int K) { return choose(N + K - 1, K); }
    T multiChoose(int N, int K, T P) { return choose(N + K - 1, K, P); }
};

// Computes the nth row of pascal's triangle
// Time Complexity:
//   no modulo: O(N)
//   modulo: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN, class T> struct PascalsRow {
    T C[MAXN];
    void init(int N) {
        T cur = 1;
        for (int j = 0; j <= N; j++) { C[j] = cur; cur = cur * (N - j) / (j + 1); }
    }
    void init(int N, T p) { // modulo prime
        assert(N < p); T cur = 1;
        for (int j = 0; j <= N; j++) { C[j] = cur; cur = divMod(cur * (N - j) % p, (j + 1), p); }
    }
};

// Computes all rows up to the nth row of pascal's triangle
// Time Complexity:
//   no modulo / modulo: O(N^2)
// Memory Complexity: O(N^2)
template <const int MAXN, class T> struct PascalsTriangle {
    T C[MAXN][MAXN];
    void init(int N) {
        C[0][0] = 1;
        for (int i = 1; i <= N; i++) {
            C[i][0] = 1;
            for (int j = 1; j <= i; j++) C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
        }
    }
    void init(int N, T m) { // modulo generic
        C[0][0] = 1;
        for (int i = 1; i <= N; i++) {
            C[i][0] = 1;
            for (int j = 1; j <= i; j++) C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % m;
        }
    }
};

// 1 + 2 + ... + n
template <class T> T sumTo(T n) { return n * (n + 1) / 2; }

// a + (a + 1) + ... + b
template <class T> sumBetween(T a, T b) { return sumTo(b) - sumTo(a - 1); }

// computes the nth term of an arithmetic sequence with starting value a1 and common difference d
template <class T, class U> arithSeq(T a1, T d, U n) { return a1 + d * (n - 1); }

// computes the sum of an arithmetic series with starting value a1 and common difference d from term a1 to term an
template <class T, class U> arithSeries(T a1, T d, U n) { return n * (a1 + arithSeq(a1, d, n)) / 2; }

// computes the nth term of a geometric sequence with starting value a1 and common ratio r
template <class T, class U> geoSeq(T a1, T r, U n) { return a1 * pow2(r, n - 1); }

// computes the sum of a geometric series with starting value a1 and common ratio r from term a1 to term an
template <class T, class U> geoSeries(T a1, T r, U n) { return r == 1 ? a1 * n : a1 * (1 - pow2(r, n)) / (1 - r); }
