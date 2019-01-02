#pragma once
#include <bits/stdc++.h>
using namespace std;

// Time Complexity of all functions: O(log max(size(a), size(b)))

using F = double; const int CUTOFF = 600, BASE = 10; const F PI = acos(-1);
template <class T> pair<T, T> operator + (const pair<T, T> &a, const pair<T, T> &b) {
    return make_pair(a.first + b.first, a.second + b.second);
}
template <class T> pair<T, T> operator - (const pair<T, T> &a, const pair<T, T> &b) {
    return make_pair(a.first - b.first, a.second - b.second);
}
template <class T> pair<T, T> operator * (const pair<T, T> &a, const pair<T, T> &b) {
    return make_pair(a.first * b.first - a.second * b.second, a.first * b.second + a.second * b.first);
}
template <class T, class U> pair<T, T> operator / (const pair<T, T> &a, const U &b) {
    return make_pair(a.first / b, a.second / b);
}

void fft(vector<pair<F, F>> &a, bool invert) {
    int N = int(a.size());
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N >> 1;
        for (; j >= bit; bit >>= 1) j -= bit;
        j += bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= N; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1); pair<F, F> wlen(cos(ang), sin(ang));
        for (int i = 0; i < N; i += len) {
            pair<F, F> w(1, 0);
            for (int j = 0; j < len / 2; j++) {
                pair<F, F> u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v; a[i + j + len / 2] = u - v; w = w * wlen;
            }
        }
    }
    if (invert) for (int i = 0; i < N; i++) a[i] = a[i] / N;
}

// Multiplies 2 big integers
template <class T> void multiplyInteger(vector<T> &a, vector<T> &b, vector<T> &res) {
    if (min(int(a.size()), int(b.size())) <= CUTOFF) {
        res.resize(int(a.size()) + int(b.size()), 0);
        for (int i = 0; i < int(a.size()); i++) for (int j = 0; j < int(b.size()); j++) res[i + j] += a[i] * b[j];
        for (int i = 0, carry = 0; i < int(res.size()); i++) { res[i] += carry; carry = res[i] / BASE; res[i] %= BASE; }
        while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
        return;
    }
    int N = int(a.size()) + int(b.size());
    while (N & (N - 1)) N++;
    vector<pair<F, F>> fa(N, make_pair(0, 0)), fb(N, make_pair(0, 0));
    for (int i = 0; i < int(a.size()); i++) fa[i] = make_pair(a[i], 0);
    for (int i = 0; i < int(b.size()); i++) fb[i] = make_pair(b[i], 0);
    fft(fa, false); fft(fb, false);
    for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i];
    fft(fa, true); res.resize(N);
    for (int i = 0, carry = 0; i < N; i++) { res[i] = (T) (fa[i].first + 0.5) + carry; carry = res[i] / BASE; res[i] %= BASE; }
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}

// Multiplies 2 polynomials
template <class T> void multiplyPolynomial(vector<T> &a, vector<T> &b, vector<T> &res) {
    if (min(int(a.size()), int(b.size())) <= CUTOFF) {
        res.resize(int(a.size()) + int(b.size()) - 1, 0);
        for (int i = 0; i < int(a.size()); i++) for (int j = 0; j < int(b.size()); j++) res[i + j] += a[i] * b[j];
        while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
        return;
    }
    int N = int(a.size()) + int(b.size()) - 1;
    while (N & (N - 1)) N++;
    vector<pair<F, F>> fa(N, make_pair(0, 0)), fb(N, make_pair(0, 0));
    for (int i = 0; i < int(a.size()); i++) fa[i] = make_pair(a[i], 0);
    for (int i = 0; i < int(b.size()); i++) fb[i] = make_pair(b[i], 0);
    fft(fa, false); fft(fb, false); bool isIntegral = is_integral<T>::value;
    for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i];
    fft(fa, true); res.resize(N);
    for (int i = 0; i < N; i++) res[i] = isIntegral ? round(fa[i].first) : fa[i].first;
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}
