#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fast Fourier Transform
// Time Complexity of multiplyInteger, multiplyPolynomial: O(N log N) where N = size(a) + size(b)

using F = long double; using C = complex<F>; const int CUTOFF = 150, DIG = 4; const F PI = acos(F(-1));

void fft(vector<C> &a) {
    int N = int(a.size()); static vector<C> rt(2, 1); static vector<int> ord;
    for (static int k = 2; k < N; k <<= 1) {
        rt.resize(N); C x = polar(F(1), PI / k);
        for (int i = k; i < (k << 1); i++) rt[i] = i & 1 ? rt[i >> 1] * x : rt[i >> 1];
    }
    if (int(ord.size()) != N) {
        ord.assign(N, 0); int len = __builtin_ctz(N);
        for (int i = 0; i < N; i++) ord[i] = (ord[i >> 1] >> 1) + ((i & 1) << (len - 1));
    }
    for (int i = 0; i < N; i++) if (i < ord[i]) swap(a[i], a[ord[i]]);
    for (int len = 1; len < N; len <<= 1) for (int i = 0; i < N; i += len << 1) for (int j = 0; j < len; j++) {
        C u = a[i + j], x = a[len + i + j], y = rt[len + j];
        C v(real(x) * real(y) - imag(x) * imag(y), real(x) * imag(y) + imag(x) * real(y));
        a[i + j] = u + v; a[len + i + j] = u - v;
    }
}

// multiplies as integers if integerMul is true, otherwise, multiplies as polynomial
template <class T> void multiply(const vector<T> &a, const vector<T> &b, vector<T> &res, bool integerMul) {
    static T BASE = pow(10LL, DIG); int N = int(a.size()) + int(b.size()) - !integerMul;
    if (max(int(a.size()), int(b.size())) <= CUTOFF) {
        vector<T> c(N, 0); T carry = 0;
        for (int i = 0; i < int(a.size()); i++) for (int j = 0; j < int(b.size()); j++) c[i + j] += a[i] * b[j];
        res.resize(N, 0); copy(c.begin(), c.end(), res.begin());
        if (integerMul) for (int i = 0; i < int(res.size()); i++) { res[i] += carry; carry = res[i] / BASE; res[i] %= BASE; }
        while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
    }
    while (N & (N - 1)) N++;
    vector<C> f(N, C(0, 0));
    for (int i = 0; i < int(a.size()); i++) f[i].real(a[i]);
    for (int i = 0; i < int(b.size()); i++) f[i].imag(b[i]);
    fft(f); C r(0, F(-0.25) / N);
    for (int i = 0; i <= N / 2; i++) {
        int j = (N - i) & (N - 1); C prod = (f[j] * f[j] - conj(f[i] * f[i])) * r; f[i] = prod; f[j] = conj(prod);
    }
    fft(f); res.resize(N); T carry = 0; bool isIntegral = is_integral<T>::value;
    if (integerMul) for (int i = 0; i < N; i++) { res[i] = T(real(f[i]) + 0.5) + carry; carry = res[i] / BASE; res[i] %= BASE; }
    else for (int i = 0; i < N; i++) res[i] = isIntegral ? round(real(f[i])) : real(f[i]);
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}
