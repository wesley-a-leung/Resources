#pragma once
#include <bits/stdc++.h>
using namespace std;

// n! % m
// Time Complexity: O(n)
template <class T> T factorial(T n, T m) {
    T ret = 1;
    for (int i = 2; i <= n; i++) ret = (ret * i) % m;
    return ret;
}

// n! % p for a prime p
// Time Complexity: O(p log n)
template <class T> T factorialPrime(T n, T p) {
    T ret = 1, h = 0;
    while (n > 1) {
        ret = (ret * ((n / p) % 2 == 1 ? p - 1 : 1)) % p; h = n % p; n /= p;
        for (int i = 2; i <= h; i++) ret = (ret * i) % p;
    }
    return ret;
}

// a * b % mod
// Time Complexity: O(log b)
template <class T> T multMod(T a, T b, T mod) {
    T x = 0, y = a % mod;
    for (; b > 0; b /= 2, y = (y + y) % mod) if (b % 2 == 1) x = (x + y) % mod;
    return x;
}

// base ^ pow % mod
// Time Complexity: O(log pow)
template <class T> T pow2(T base, T pow) {
    T x = 1, y = base;
    for (; pow > 0; pow /= 2, y = y * y) if (pow % 2 == 1) x = x * y;
    return x;
}

// base ^ pow % mod
// Time Complexity: O(log pow)
template <class T> T powMod(T base, T pow, T mod) {
    T x = 1, y = base % mod;
    for (; pow > 0; pow /= 2, y = y * y % mod) if (pow % 2 == 1) x = x * y % mod;
    return x;
}

// Modular Multiplicative Inverse of i in Zp for a prime p
// Time Complexity: O(log p)
template <class T> T multInv(T i, T p) {
    return powMod(i % p, p - 2, p);
}

// i / j % p for a prime p
// Time Complexity: O(log p)
template <class T> T divMod(T i, T j, T p) {
    return i % p * powMod(j % p, p - 2, p) % p;
}

// n choose k
// Time Complexity: O(min(k, n - k))
long long choose(int n, int k) {
    if (n < k) return 0;
    if (k > n - k) k = n - k;
    long long ret = 1;
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
template <class T> T fastChoose(int n, int k, T p) {
    return divMod(divMod(factorial(n, p), factorial(k, p), p), factorial(n - k, p), p);
}

// choosing k elements from n items with replacement, modulo p
// Time Complexity: O(log p) if factorials are precomputed
template <class T> T multiChoose(int n, int k, T p) {
    return fastChoose(n + k - 1, k, p);
}

// n permute k
// Time Complexity: O(min(k, n - k))
long long permute(int n, int k) {
    if (n < k) return 0;
    if (k > n - k) k = n - k;
    long long ret = 1;
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
template <class T> T fastPermute(int n, int k, T p) {
    return divMod(factorial(n, p), factorial(n - k, p), p);
}

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
    void init(int N, T P) { // compute factorials mod P up to N!
        assert(N < P); fact[0] = 1;
        for (int i = 1; i <= N; i++) fact[i] = fact[i - 1] * i % P;
        invFact[N] = multInv(fact[N], P);
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
        assert(p > N); T cur = 1;
        for (int j = 0; j <= N; j++) {
            C[j] = cur;
            cur = divMod(cur * (N - j) % p, (j + 1), p);
        }
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
