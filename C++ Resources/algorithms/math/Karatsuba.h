#pragma once
#include <bits/stdc++.h>
using namespace std;

// Karatsuba Multiplication
// Time Complexity of multiplyInteger, multiplyPolynomial: O(N ^ log_2(3)) where N = size(a) + size(b)

const int CUTOFF = 32, DIG = 1;

template <class T> T pow2(T base, T pow) {
    T x = 1, y = base;
    for (; pow > 0; pow /= 2, y = y * y) if (pow % 2 == 1) x = x * y;
    return x;
}

template <class T> vector<T> karatsuba(const vector<T> &a, const vector<T> &b) {
    int n = int(a.size());
    if (n <= CUTOFF) {
        vector<T> res(n + n);
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) res[i + j] += a[i] * b[j];
        return res;
    }
    int k = n >> 1; vector<T> a2(a.begin() + k, a.end()), b2(b.begin() + k, b.end()), a2b2 = karatsuba(a2, b2);
    for (int i = 0; i < k; i++) { a2[i] += a[i]; b2[i] += b[i]; }
    vector<T> r = karatsuba(a2, b2), a1b1 = karatsuba(vector<T>(a.begin(), a.begin() + k), vector<T>(b.begin(), b.begin() + k)), res(n << 1);
    for (int i = 0; i < int(r.size()); i++) { res[i] += a1b1[i]; res[i + k] += r[i] - a1b1[i] - a2b2[i]; res[i + n] += a2b2[i]; }
    return res;
}

// Multiplies 2 big integers
template <class T> vector<T> multiplyInteger(vector<T> &a, vector<T> &b) {
    while (int(a.size()) < int(b.size())) a.push_back(0);
    while (int(b.size()) < int(a.size())) b.push_back(0);
    while (int(a.size()) & (int(a.size()) - 1)) { a.push_back(0); b.push_back(0); }
    vector<T> c = karatsuba(a, b); T carry = 0; static T BASE = pow2(T(10), T(DIG));
    for (int i = 0; i < int(c.size()); i++) { T cur = c[i] + carry; c[i] = T(cur % BASE); carry = T(cur / BASE); }
    while (int(c.size()) > 1 && c.back() == 0) c.pop_back();
    return c;
}

// Multiplies 2 polynomials
template <class T> vector<T> multiplyPolynomial(vector<T> &a, vector<T> &b) {
    while (int(a.size()) < int(b.size())) a.push_back(0);
    while (int(b.size()) < int(a.size())) b.push_back(0);
    while (int(a.size()) & (int(a.size()) - 1)) { a.push_back(0); b.push_back(0); }
    vector<T> c = karatsuba(a, b);
    while (int(c.size()) > 1 && c.back() == 0) c.pop_back();
    return c;
}