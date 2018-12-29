#pragma once
#include <bits/stdc++.h>
using namespace std;

// Time Complexity of all functions: O(log max(size(a), size(b)))

const double PI = 3.14159265358979323846;
template <class T> pair<T, T> operator + (const pair<T, T> &a, const pair<T, T> &b) {
    return make_pair(a.first + b.first, a.second + b.second);
}
template <class T> pair<T, T> operator - (const pair<T, T> &a, const pair<T, T> &b) {
    return make_pair(a.first - b.first, a.second - b.second);
}
template <class T> pair<T, T> operator * (const pair<T, T> &a, const pair<T, T> &b) {
    return make_pair(a.first * b.first - a.second * b.second, a.first * b.second + a.second * b.first);
}
template <class T> pair<T, T> operator / (const pair<T, T> &a, const T &b) {
    return make_pair(a.first / b, a.second / b);
}

void fft(vector<pair<double, double>> &a, bool invert) {
    int N = int(a.size());
    for (int i = 1, j = 0; i < N; i++) {
        int bit = N >> 1;
        for (; j >= bit; bit >>= 1) j -= bit;
        j += bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= N; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        pair<double, double> wlen(cos(ang), sin(ang));
        for (int i = 0; i < N; i += len) {
            pair<double, double> w(1, 0);
            for (int j = 0; j < len / 2; j++) {
                pair<double, double> u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v; a[i + j + len / 2] = u - v; w = w * wlen;
            }
        }
    }
    if (invert) for (int i = 0; i < N; i++) a[i] = a[i] / double(N);
}

void multiplyInteger(vector<int> &a, vector<int> &b, vector<int> &res) {
    int N = int(a.size()) + int(b.size());
    while (N & (N - 1)) N++;
    vector<pair<double, double>> fa(N, make_pair(0, 0)), fb(N, make_pair(0, 0));
    for (int i = 0; i < int(a.size()); i++) fa[i] = make_pair(a[i], 0);
    for (int i = 0; i < int(b.size()); i++) fb[i] = make_pair(b[i], 0);
    fft(fa, false); fft(fb, false);
    for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i];
    fft(fa, true); res.resize(N);
    for (int i = 0, carry = 0; i < N; i++) { res[i] = (int) (fa[i].first + 0.5) + carry; carry = res[i] / 10; res[i] %= 10; }
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}

// Multiplies 2 polynomials
template <class T> void multiplyPolynomial(vector<T> &a, vector<T> &b, vector<T> &res) {
    int N = int(a.size()) + int(b.size()) - 1;
    while (N & (N - 1)) N++;
    vector<pair<double, double>> fa(N, make_pair(0, 0)), fb(N, make_pair(0, 0));
    for (int i = 0; i < int(a.size()); i++) fa[i] = make_pair(a[i], 0);
    for (int i = 0; i < int(b.size()); i++) fb[i] = make_pair(b[i], 0);
    fft(fa, false); fft(fb, false); bool isIntegral = is_integral<T>::value;
    for (int i = 0; i < N; i++) fa[i] = fa[i] * fb[i];
    fft(fa, true); res.resize(N);
    for (int i = 0; i < N; i++) res[i] = isIntegral ? round(fa[i].first) : fa[i].first;
    while (int(res.size()) > 1 && res.back() == 0) res.pop_back();
}
