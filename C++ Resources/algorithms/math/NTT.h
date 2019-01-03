#pragma once
#include <bits/stdc++.h>
using namespace std;

// Time Complexity of all functions: O(N log N) where N = max(size(a), size(b))

using T = long long; const int CUTOFF = 600, BASE = 10;

template <class T> T powMod(T base, T pow, T mod) {
    T x = 1, y = base % mod;
    for (; pow > 0; pow /= 2, y = y * y % mod) if (pow % 2 == 1) x = x * y % mod;
    return x;
}

template <class T> T multInv(T i, T p) {
    return powMod(i % p, p - 2, p);
}

T primitiveRoot(T p) {
    vector<T> fact; T phi = p - 1, n = phi;
    for (T i = 2; i * i <= n; i++) {
        if (n % i == 0) fact.push_back(i);
        while(n % i == 0) n /= i;
    }
    if (n > 1) fact.push_back(n);
    for (T res = 2; res <= p; res++) {
        bool ok = true;
        for (int i = 0; i < int(fact.size()) && ok; i++) ok &= powMod(res, phi / fact[i], p) != 1;
        if (ok) return res;
    }
    assert(false); return -1;
}

// MOD = C * 2 ^ K + 1
// for MOD < 1e9:
//   C = 119, K = 23 (MOD = 998244353)
//   C = 5, K = 25
//   C = 7, K = 26
// for MOD > 1e9:
//   C = 479, K = 21
//   C = 483, K = 21
const T C = 479, K = 21, PK = 1 << K, MOD = C * PK + 1, ROOT = powMod(primitiveRoot(MOD), C, MOD), INVROOT = multInv(ROOT, MOD);

void fft(vector<T> &a, bool invert) {
    int N = int(a.size());
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N >> 1;
        for (; j >= bit; bit >>= 1) j -= bit;
        j += bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= N; len <<= 1) {
        T wlen = invert ? INVROOT : ROOT;
        for (int i = len; i < PK; i <<= 1) wlen = wlen * wlen % MOD;
        for (int i = 0; i < N; i += len) {
            T w = 1;
            for (int j = 0; j < len / 2; j++) {
                T u = a[i + j], v = a[i + j + len / 2] * w % MOD;
                a[i + j] = u + v < MOD ? u + v : u + v - MOD; a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + MOD; w = w * wlen % MOD;
            }
        }
    }
    if (invert) {
        T invN = multInv(T(N), MOD);
        for (int i = 0; i < N; i++) a[i] = a[i] * invN % MOD;
    }
}

// Multiplies 2 big integers
template <class T> void multiplyInteger(vector<T> &a, vector<T> &b, vector<T> &res) {
    static_assert(is_integral<T>::value, "T must be an integral type");
    if (min(int(a.size()), int(b.size())) <= CUTOFF) {
        res.resize(int(a.size()) + int(b.size()), 0); T carry = 0;
        for (int i = 0; i < int(a.size()); i++) for (int j = 0; j < int(b.size()); j++) {
            res[i + j] += a[i] * b[j] % MOD;
            if (res[i + j] >= MOD) res[i + j] -= MOD;
        }
        for (int i = 0; i < int(res.size()); i++) { res[i] += carry; carry = res[i] / BASE; res[i] %= BASE; }
        while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
        return;
    }
    int N = int(a.size()) + int(b.size());
    while (N & (N - 1)) N++;
    vector<T> fa(N, 0), fb(N, 0); copy(a.begin(), a.end(), fa.begin()); copy(b.begin(), b.end(), fb.begin()); fft(fa, false); fft(fb, false);
    for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i] % MOD;
    fft(fa, true); res.resize(N); T carry = 0;
    for (int i = 0; i < N; i++) { res[i] = fa[i] + carry; carry = res[i] / BASE; res[i] %= BASE; }
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}

// Multiplies 2 polynomials
template <class T> void multiplyPolynomial(vector<T> &a, vector<T> &b, vector<T> &res) {
    static_assert(is_integral<T>::value, "T must be an integral type");
    if (min(int(a.size()), int(b.size())) <= CUTOFF) {
        res.resize(int(a.size()) + int(b.size()) - 1, 0);
        for (int i = 0; i < int(a.size()); i++) for (int j = 0; j < int(b.size()); j++) {
            res[i + j] += a[i] * b[j] % MOD;
            if (res[i + j] >= MOD) res[i + j] -= MOD;
        }
        return;
    }
    int N = int(a.size()) + int(b.size()) - 1;
    while (N & (N - 1)) N++;
    vector<T> fa(N, 0), fb(N, 0); copy(a.begin(), a.end(), fa.begin()); copy(b.begin(), b.end(), fb.begin()); fft(fa, false); fft(fb, false);
    res.resize(N);
    for (int i = 0; i < N; i++) res[i] = fa[i] * fb[i] % MOD;
    fft(res, true);
}
