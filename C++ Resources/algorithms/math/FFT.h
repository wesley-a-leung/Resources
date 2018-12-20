#pragma once
#include <bits/stdc++.h>
using namespace std;

// Time Complexity of all functions: O(log max(size(a), size(b)))

const double PI = 3.14159265358979323846;

void fft(vector<complex<double>> &a, bool invert) {
    int N = int(a.size());
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N >> 1;
        for (; j >= bit; bit >>= 1) j -= bit;
        j += bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= N; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        complex<double> wlen(cos(ang), sin(ang));
        for (int i = 0; i < N; i += len) {
            complex<double> w(1, 0);
            for (int j = 0; j < len / 2; j++) {
                complex<double> u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v; a[i + j + len / 2] = u - v; w *= wlen;
            }
        }
    }
    if (invert) for (int i = 0; i < N; i++) a[i] /= N;
}

// Multiplies 2 large integers
void multiplyInteger(vector<int> &a, vector<int> &b, vector<int> &res) {
    vector<complex<double>> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int N = int(a.size()) + int(b.size());
    while (N & (N - 1)) N++;
    fa.resize(N); fb.resize(N); fft(fa, false); fft(fb, false);
    for (int i = 0; i < N; i++) fa[i] *= fb[i];
    fft(fa, true); res.resize(N);
    int carry = 0;
    for (int i = 0; i < N; i++) { res[i] = (int) (fa[i].real() + 0.5) + carry; carry = res[i] / 10; res[i] %= 10; }
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}

// Multiplies 2 polynomial
template <class T> void multiplyPolynomial(vector<T> &a, vector<T> &b, vector<T> &res) {
    vector<complex<double>> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int N = int(a.size()) + int(b.size()) - 1; bool isIntegral = is_integral<T>::value;
    while (N & (N - 1)) N++;
    fa.resize(N); fb.resize(N); fft(fa, false); fft(fb, false);
    for (int i = 0; i < N; i++) fa[i] *= fb[i];
    fft(fa, true); res.resize(N);
    for (int i = 0; i < N; i++) res[i] = isIntegral ? round(real(fa[i])) : real(fa[i]);
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}
