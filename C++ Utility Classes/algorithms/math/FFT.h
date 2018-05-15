#ifndef ALGORITHMS_MATH_FFT_H_
#define ALGORITHMS_MATH_FFT_H_

#include <bits/stdc++.h>
using namespace std;

const double PI = 3.14159265358979323846;

void fft(vector<complex<double>> &a, bool invert) {
    int N = (int) a.size();
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
                complex<double> u = a[i + j];
                complex<double> v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) for (int i = 0; i < N; i++) a[i] /= N;
}

/**
 * Multiplies two (large) integers together. num[i] = the ith digit where 0 is the ones digit
 *
 * @param a the first integer
 * @param b the second integer
 * @param res the product of a and b
 */
void multiplyInteger(vector<int> &a, vector<int> &b, vector<int> &res) {
    vector<complex<double>> fa(a.begin(), a.end());
    vector<complex<double>> fb(b.begin(), b.end());
    int N = (int) (a.size()) + (int) (b.size());
    while (N & (N - 1)) N++;
    fa.resize(N);
    fb.resize(N);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < N; i++) fa[i] *= fb[i];
    fft(fa, true);
    res.resize(N);
    int carry = 0;
    for (int i = 0; i < N; i++) {
        res[i] = (int) (fa[i].real() + 0.5) + carry;
        carry = res[i] / 10;
        res[i] %= 10;
    }
    while (((int) res.size()) > 1 && res.back() == 0) res.pop_back();
}

/**
 * Multiplies two real polynomials together. vec[i] = coefficient of the ith exponent term.
 *
 * @param a the first polynomial
 * @param b the second polynomial
 * @param res the product of a and b
 */
void multiplyPolynomial(vector<double> &a, vector<double> &b, vector<double> &res) {
    vector<complex<double>> fa(a.begin(), a.end());
    vector<complex<double>> fb(b.begin(), b.end());
    int N = (int) (a.size()) + (int) (b.size()) - 1;
    while (N & (N - 1)) N++;
    fa.resize(N);
    fb.resize(N);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < N; i++) fa[i] *= fb[i];
    fft(fa, true);
    res.resize(N);
    for (int i = 0; i < N; i++) res[i] = real(fa[i]); // round for integral types
    while (((int) res.size()) > 1 && res.back() == 0) res.pop_back();
}

#endif /* ALGORITHMS_MATH_FFT_H_ */
