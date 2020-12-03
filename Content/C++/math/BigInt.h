#pragma once
#include <bits/stdc++.h>
using namespace std;

struct BigInt {
    using T = long long; using F = long double;
    static const constexpr int DIG = 4;
    static const constexpr T BASE = pow(10, DIG);
    static const constexpr int KARATSUBA_CUTOFF = 64;
    static const constexpr F PI = acos(F(-1));
    vector<T> a; int sign;
    BigInt() : sign(1) {}
    BigInt(T v) { *this = v; }
    BigInt(const string &s) { read(s); }
    BigInt operator + (const BigInt &v) const {
        if (sign == v.sign) {
            BigInt res = v;
            T carry = 0;
            for (int i = 0; i < max(int(a.size()), int(v.a.size())) || carry; i++) {
                if (i == int(res.a.size())) res.a.push_back(0);
                res.a[i] += carry + (i < int(a.size()) ? a[i] : 0); carry = res.a[i] >= BASE;
                if (carry) res.a[i] -= BASE;
            }
            return res;
        }
        return *this - (-v);
    }
    BigInt operator - (const BigInt &v) const {
        if (sign == v.sign) {
            if (abs() >= v.abs()) {
                BigInt res = *this;
                T carry = 0;
                for (int i = 0; i < int(v.a.size()) || carry; i++) {
                    res.a[i] -= carry + (i < int(v.a.size()) ? v.a[i] : 0); carry = res.a[i] < 0;
                    if (carry) res.a[i] += BASE;
                }
                res.trim(); return res;
            }
            return -(v - *this);
        }
        return *this + (-v);
    }
    void operator *= (T v) {
        if (v < 0) sign = -sign, v = -v;
        T carry = 0;
        for (int i = 0; i < int(a.size()) || carry; i++) {
            if (i == int(a.size())) a.push_back(0);
            T cur = a[i] * v + carry; carry = cur / BASE; a[i] = cur % BASE;
        }
        trim();
    }
    BigInt operator * (T v) const { BigInt res = *this; res *= v; return res; }
    friend pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
        T norm = BASE / (b1.a.back() + 1); BigInt a = a1.abs() * norm, b = b1.abs() * norm, q, r; q.a.resize(int(a.a.size()));
        for (int i = int(a.a.size()) - 1; i >= 0; i--) {
            r *= BASE; r += a.a[i];
            T s1 = int(r.a.size()) <= int(b.a.size()) ? 0 : r.a[int(b.a.size())];
            T s2 = int(r.a.size()) <= int(b.a.size()) - 1 ? 0 : r.a[int(b.a.size()) - 1];
            T d = (BASE * s1 + s2) / b.a.back();
            r -= b * d;
            while (r < 0) r += b, --d;
            q.a[i] = d;
        }
        q.sign = a1.sign * b1.sign; r.sign = a1.sign; q.trim(); r.trim(); return make_pair(q, r / norm);
    }
    BigInt operator / (const BigInt &v) const { return divmod(*this, v).first; }
    BigInt operator % (const BigInt &v) const { return divmod(*this, v).second; }
    void operator /= (T v) {
        if (v < 0) sign = -sign, v = -v;
        for (int i = int(a.size()) - 1, rem = 0; i >= 0; i--) {
            T cur = a[i] + rem * BASE; a[i] = cur / v; rem = cur % v;
        }
        trim();
    }
    BigInt operator / (T v) const { BigInt res = *this; res /= v; return res; }
    T operator % (T v) const {
        if (v < 0) v = -v;
        T m = 0;
        for (int i = int(a.size()) - 1; i >= 0; i--) m = (a[i] + m * BASE) % v;
        return m * sign;
    }
    void operator += (const BigInt &v) { *this = *this + v; }
    void operator -= (const BigInt &v) { *this = *this - v; }
    void operator *= (const BigInt &v) { *this = *this * v; }
    void operator /= (const BigInt &v) { *this = *this / v; }
    bool operator < (const BigInt &v) const {
        if (sign != v.sign) return sign < v.sign;
        if (int(a.size()) != int(v.a.size())) return int(a.size()) * sign < int(v.a.size()) * v.sign;
        for (int i = int(a.size()) - 1; i >= 0; i--) if (a[i] != v.a[i]) return a[i] * sign < v.a[i] * sign;
        return false;
    }
    bool operator > (const BigInt &v) const { return v < *this; }
    bool operator <= (const BigInt &v) const { return !(v < *this); }
    bool operator >= (const BigInt &v) const { return !(*this < v); }
    bool operator == (const BigInt &v) const { return !(*this < v) && !(v < *this); }
    bool operator != (const BigInt &v) const { return *this < v || v < *this; }
    void trim() {
        while (!a.empty() && a.back() == 0) a.pop_back();
        if (a.empty()) sign = 1;
    }
    bool isZero() const { return a.empty() || (int(a.size()) == 1 && !a[0]); }
    BigInt operator - () const { BigInt res = *this; res.sign = -sign; return res; }
    BigInt abs() const { BigInt res = *this; res.sign *= res.sign; return res; }
    T value() const { 
        T res = 0;
        for (int i = int(a.size()) - 1; i >= 0; i--) res = res * BASE + a[i];
        return res * sign;
    }
    friend BigInt gcd(const BigInt &a, const BigInt &b) { return b.isZero() ? a : gcd(b, a % b); }
    friend BigInt lcm(const BigInt &a, const BigInt &b) { return a / gcd(a, b) * b; }
    void read(const string &s) {
        sign = 1; a.clear(); int pos = 0;
        for (; pos < int(s.size()) && (s[pos] == '-' || s[pos] == '+'); ++pos) if (s[pos] == '-') sign = -sign;
        for (int i = int(s.size()) - 1; i >= pos; i -= DIG) {
            T x = 0;
            for (int j = max(pos, i - DIG + 1); j <= i; j++) x = x * 10 + s[j] - '0';
            a.push_back(x);
        }
        trim();
    }
    friend istream& operator >> (istream &stream, BigInt &v) { string s; stream >> s; v.read(s); return stream; }
    string write() const {
        string ret = "";
        if (sign == -1) ret.push_back('-');
        for (char c : to_string(a.empty() ? 0 : a.back())) ret.push_back(c);
        for (int i = int(a.size()) - 2; i >= 0; i--) {
            string s = to_string(a[i]);
            for (int j = int(s.size()); j < DIG; j++) ret.push_back('0');
            for (char c : s) ret.push_back(c);
        }
        return ret;
    }
    friend ostream& operator << (ostream &stream, const BigInt &v) { stream << v.write(); return stream; }
    void fft(vector<complex<F>> &a) const {
        int N = int(a.size());
        vector<complex<F>> roots = {complex<F>(0, 0), complex<F>(1, 0)};
        int len = __builtin_ctz(int(roots.size())); roots.resize(N);
        for (; (1 << len) < N; len++) {
            F mnAngle = 2 * PI / (1 << (len + 1));
            for (int i = 0; i < (1 << (len - 1)); i++) {
                int ind = (1 << (len - 1)) + i; F ang = mnAngle * (2 * i + 1);
                roots[2 * ind] = roots[ind]; roots[2 * ind + 1] = complex<F>(cos(ang), sin(ang));
            }
        }
        vector<int> ord(N, 0); len = __builtin_ctz(N);
        for (int i = 0; i < N; i++) ord[i] = (ord[i >> 1] >> 1) + ((i & 1) << (len - 1));
        for (int i = 0; i < N; i++) if (i < ord[i]) swap(a[i], a[ord[i]]);
        for (int len = 1; len < N; len <<= 1) for (int i = 0; i < N; i += len << 1) for (int j = 0; j < len; j++) {
            complex<F> u = a[i + j], v = a[len + i + j] * roots[len + j]; a[i + j] = u + v; a[len + i + j] = u - v;
        }
    }
    void multiply_fft(const vector<T> &a, const vector<T> &b, vector<T> &res) const {
        int N = int(a.size()) + int(b.size());
        while (N & (N - 1)) N++;
        vector<complex<F>> f(N, 0);
        for (int i = 0; i < int(a.size()); i++) f[i].real(a[i]);
        for (int i = 0; i < int(b.size()); i++) f[i].imag(b[i]);
        fft(f); complex<F> r(0, F(-0.25) / N);
        for (int i = 0; i <= N / 2; i++) {
            int j = (N - i) & (N - 1);
            complex<F> prod = (f[j] * f[j] - conj(f[i] * f[i])) * r; f[i] = prod; f[j] = conj(prod);
        }
        fft(f); res.resize(N); T carry = 0;
        for (int i = 0; i < N; i++) { res[i] = T(f[i].real() + 0.5) + carry; carry = res[i] / BASE; res[i] %= BASE; }
    }
    BigInt operator * (const BigInt &v) const {
        BigInt res; res.sign = sign * v.sign; multiply_fft(a, v.a, res.a); res.trim(); return res;
    }
    BigInt mul_simple(const BigInt &v) const {
        BigInt res; res.sign = sign * v.sign; res.a.resize(int(a.size()) + int(v.a.size()));
        for (int i = 0; i < int(a.size()); i++) if (a[i]) {
            T carry = 0;
            for (int j = 0; j < int(v.a.size()) || carry; j++) {
                T cur = res.a[i + j] + a[i] * (j < int(v.a.size()) ? v.a[j] : 0) + carry;
                carry = cur / BASE; res.a[i + j] = cur % BASE;
            }
        }
        res.trim(); return res;
    }
    template <class T, class ItA, class ItB, class ItRes> static void karatsuba(int n, ItA a, ItB b, ItRes res) {
        if (n <= KARATSUBA_CUTOFF) {
            fill(res, res + n * 2, 0);
            for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) res[i + j] += a[i] * b[j];
            return;
        }
        int k = n / 2; vector<T> tmp(n, 0), c(n, 0); auto atmp = tmp.begin(), btmp = atmp + k;
        for (int i = 0; i < k; i++) { atmp[i] = a[i] + a[i + k]; btmp[i] = b[i] + b[i + k]; }
        karatsuba<T>(k, atmp, btmp, c.begin()); karatsuba<T>(k, a, b, res); karatsuba<T>(k, a + k, b + k, res + n);
        for (int i = 0; i < k; i++) { T t = res[i + k]; res[i + k] += c[i] - res[i] - res[i + k * 2]; res[i + k * 2] += c[i + k] - t - res[i + k * 3]; }
    }
    BigInt mul_karatsuba(const BigInt &v) const {
        vector<T> a(this->a.begin(), this->a.end()), b(v.a.begin(), v.a.end());
        int n = max(a.size(), b.size());
        while (n & (n - 1)) n++;
        a.resize(n, 0); b.resize(n, 0); vector<T> c(n * 2, 0); karatsuba<T>(n, a.begin(), b.begin(), c.begin());
        T carry = 0; BigInt res; res.sign = sign * v.sign;
        for (int i = 0; i < n * 2; i++) { T cur = c[i] + carry; res.a.push_back(cur % BASE); carry = T(cur / BASE); }
        res.trim(); return res;
    }
};
