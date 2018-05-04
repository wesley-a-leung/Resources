#ifndef ALGORITHMS_MATH_FFT_H_
#define ALGORITHMS_MATH_FFT_H_

#include <bits/stdc++.h>
using namespace std;

const double PI = 3.14159265358979323846;

/**
 * Computes the Nth roots of unity.
 *
 * @param N the root
 * @param out the Nth roots of unity
 */
void getRoots(int N, vector<complex<double>> &out) {
    for (int i = 0; i < N; i++) out.push_back(polar(1.0, 2.0 * PI * i / N));
}

template<typename T> void fft(T *in, complex<double> *out, vector<complex<double>> &roots, int len, int step = 1) {
    if (len == 1) *out = *in;
    else {
        fft(in, out, roots, len / 2, step * 2); // even
        fft(in + step, out + len / 2, roots, len / 2, step * 2); // odd
        for (int i = 0, j = 0; i < len / 2; i++, j += step) {
            auto temp = out[i + len / 2] * roots[j];
            out[i + len / 2] = out[i] - temp;
            out[i] += temp;
        }
    }
}

/**
 * Computes the Fast Fourier Transformation of the vector in.
 *
 * @param in the input vector (of any type)
 * @param out the output vector of type complex<double>
 * @param roots the complex roots of unity
 */
template<typename T> void fft(vector<T> &in, vector<complex<double>> &out, vector<complex<double>> &roots) {
    assert(in.size() == out.size());
    int N = (int) in.size();
    assert(!(N & (N - 1)));
    fft(in.data(), out.data(), roots, N);
}

/**
 * Computes the inverse Fast Fourier Transformation of the vector in.
 *
 * @param in the input vector (of any type)
 * @param out the output vector of type complex<double>
 * @param invRoots the conjugate of the complex roots of unity
 */
template<typename T> void invfft(vector<T> &in, vector<complex<double>> &out, vector<complex<double>> &invRoots) {
    assert(in.size() == out.size());
    int N = (int) in.size();
    assert(!(N & (N - 1)));
    fft(in, out, invRoots);
    for (int i = 0; i < N; i++) out[i] *= (1.0 / N);
}

/**
 * Computes the convolution of two vectors (of any type)
 *
 * @param a the first input vector (of any type)
 * @param b the second input vector (of any type)
 * @param out the output vector of type complex<double>
 */
template<typename T> void convolve(vector<T> &a, vector<T> &b, vector<complex<double>> &out) {
    assert(a.size() == b.size() && a.size() == out.size());
    int N = (int) a.size();
    assert(!(N & (N - 1)));
    vector<complex<double>> x(N), y(N), roots;
    getRoots(N, roots);
    fft(a, x, roots);
    fft(b, y, roots);
    for (int i = 0; i < N; i++) {
        x[i] *= y[i];
        roots[i] = conj(roots[i]);
    }
    invfft(x, y, roots);
    for (int i = 0; i < N; i++) out[i] = y[i];
}

/**
 * Multiplies two real polynomials together. vec[i] = coefficient of the ith exponent term.
 *
 * @param a the first polynomial
 * @param b the second polynomial
 * @param out the product of a and b
 */
void multiplyPolynomial(vector<double> a, vector<double> b, vector<double> &out) {
    int N = (int) (a.size()) + (int) (b.size()) - 1;
    while (N & (N - 1)) N++;
    a.resize(N);
    b.resize(N);
    vector<complex<double>> c(N);
    convolve(a, b, c);
    out.resize(N);
    for (int i = 0; i < N; i++) out[i] = real(c[i]);
    while ((int) (out.size()) > 1 && out.back() == 0) out.pop_back();
}

/**
 * Multiplies two (large) integers together. num[i] = the ith digit where 0 is the ones digit
 *
 * @param a the first integer
 * @param b the second integer
 * @param out the product of a and b
 */
void multiplyInteger(vector<int> a, vector<int> b, vector<int> &out) {
    int N = (int) (a.size()) + (int) (b.size());
    while (N & (N - 1)) N++;
    a.resize(N);
    b.resize(N);
    vector<complex<double>> c(N);
    convolve(a, b, c);
    out.resize(N);
    for (int i = 0; i < N; i++) out[i] = (int) round(real(c[i]));
    for (int i = 0; i < (int) (out.size()); i++) {
        if (out[i] >= 10) {
            if (i == (int) (out.size()) - 1) out.push_back(out[i] / 10);
            else out[i + 1] += out[i] / 10;
            out[i] %= 10;
        }
    }
    while ((int) (out.size()) > 1 && out.back() == 0) out.pop_back();
}

#endif /* ALGORITHMS_MATH_FFT_H_ */
