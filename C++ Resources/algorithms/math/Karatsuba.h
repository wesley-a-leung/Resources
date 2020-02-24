#pragma once
#include <bits/stdc++.h>
#include "Combinatorics.h"
using namespace std;

// Karatsuba Multiplication
// Time Complexity of multiplyInteger, multiplyPolynomial: O(N ^ log_2(3)) where N = size(a) + size(b)

const int CUTOFF = 64, DIG = 1;

template <class T, class ItA, class ItB, class ItRes> void karatsuba(int n, ItA a, ItB b, ItRes res) {
    if (n <= CUTOFF) {
        fill(res, res + n * 2, 0);
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) res[i + j] += a[i] * b[j];
        return;
    }
    int k = n / 2; vector<T> tmp(n, 0), c(n, 0); auto atmp = tmp.begin(), btmp = atmp + k;
    for (int i = 0; i < k; i++) { atmp[i] = a[i] + a[i + k]; btmp[i] = b[i] + b[i + k]; }
    karatsuba<T>(k, atmp, btmp, c.begin()); karatsuba<T>(k, a, b, res); karatsuba<T>(k, a + k, b + k, res + n);
    for (int i = 0; i < k; i++) { T t = res[i + k]; res[i + k] += c[i] - res[i] - res[i + k * 2]; res[i + k * 2] += c[i + k] - t - res[i + k * 3]; }
}

// Multiplies 2 big integers
template <class T> void multiplyInteger(vector<T> a, vector<T> b, vector<T> &res) {
    int n = max(a.size(), b.size());
    while (n & (n - 1)) n++;
    a.resize(n, 0); b.resize(n, 0); res.resize(n * 2, 0); karatsuba<T>(n, a.begin(), b.begin(), res.begin());
    T carry = 0; static T BASE = pow2(T(10), DIG);
    for (int i = 0; i < n * 2; i++) { T cur = res[i] + carry; res[i] = T(cur % BASE); carry = T(cur / BASE); }
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}

// Multiplies 2 polynomials
template <class T> void multiplyPolynomial(vector<T> a, vector<T> b, vector<T> &res) {
    int n = max(a.size(), b.size());
    while (n & (n - 1)) n++;
    a.resize(n, 0); b.resize(n, 0); res.resize(n * 2, 0); karatsuba<T>(n, a.begin(), b.begin(), res.begin());
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}
