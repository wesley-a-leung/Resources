#pragma once
#include <bits/stdc++.h>
using namespace std;

struct BigInt {
    static const constexpr int KARATSUBA_CUTOFF = 32;
    static const constexpr long double PI = acosl(-1);
    vector<int> a; int sign;
    BigInt() : sign(1) {}
    BigInt(long long v) { *this = v; }
    BigInt(const string &s) { read(s); }
    void operator = (const BigInt &v) { sign = v.sign; a = v.a; }
    void operator = (long long v) {
        sign = 1;
        if (v < 0) sign = -1, v = -v;
        a.clear();
        for (; v > 0; v = v / 10) a.push_back(v % 10);
    }
    BigInt operator + (const BigInt &v) const {
        if (sign == v.sign) {
            BigInt res = v;
            for (int i = 0, carry = 0; i < max(int(a.size()), int(v.a.size())) || carry; i++) {
                if (i == int(res.a.size())) res.a.push_back(0);
                res.a[i] += carry + (i < int(a.size()) ? a[i] : 0); carry = res.a[i] >= 10;
                if (carry) res.a[i] -= 10;
            }
            return res;
        }
        return *this - (-v);
    }
    BigInt operator - (const BigInt &v) const {
        if (sign == v.sign) {
            if (abs() >= v.abs()) {
                BigInt res = *this;
                for (int i = 0, carry = 0; i < int(v.a.size()) || carry; i++) {
                    res.a[i] -= carry + (i < int(v.a.size()) ? v.a[i] : 0); carry = res.a[i] < 0;
                    if (carry) res.a[i] += 10;
                }
                res.trim(); return res;
            }
            return -(v - *this);
        }
        return *this + (-v);
    }
    void operator *= (int v) {
        if (v < 0) sign = -sign, v = -v;
        for (int i = 0, carry = 0; i < int(a.size()) || carry; i++) {
            if (i == int(a.size())) a.push_back(0);
            long long cur = a[i] * (long long) v + carry; carry = (int) (cur / 10); a[i] = (int) (cur % 10);
        }
        trim();
    }
    BigInt operator * (int v) const { BigInt res = *this; res *= v; return res; }
    friend pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
        int norm = 10 / (b1.a.back() + 1); BigInt a = a1.abs() * norm, b = b1.abs() * norm, q, r; q.a.resize(int(a.a.size()));
        for (int i = int(a.a.size()) - 1; i >= 0; i--) {
            r *= 10; r += a.a[i];
            int s1 = int(r.a.size()) <= int(b.a.size()) ? 0 : r.a[int(b.a.size())];
            int s2 = int(r.a.size()) <= int(b.a.size()) - 1 ? 0 : r.a[int(b.a.size()) - 1];
            int d = ((long long) 10 * s1 + s2) / b.a.back();
            r -= b * d;
            while (r < 0) r += b, --d;
            q.a[i] = d;
        }
        q.sign = a1.sign * b1.sign; r.sign = a1.sign; q.trim(); r.trim(); return make_pair(q, r / norm);
    }
    BigInt operator / (const BigInt &v) const { return divmod(*this, v).first; }
    BigInt operator % (const BigInt &v) const { return divmod(*this, v).second; }
    void operator /= (int v) {
        if (v < 0) sign = -sign, v = -v;
        for (int i = int(a.size()) - 1, rem = 0; i >= 0; i--) {
            long long cur = a[i] + rem * (long long) 10; a[i] = (int) (cur / v); rem = (int) (cur % v);
        }
        trim();
    }
    BigInt operator / (int v) const { BigInt res = *this; res /= v; return res; }
    int operator % (int v) const {
        if (v < 0) v = -v;
        int m = 0;
        for (int i = int(a.size()) - 1; i >= 0; i--) m = (a[i] + m * (long long) 10) % v;
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
    long long longValue() const { 
        long long res = 0;
        for (int i = int(a.size()) - 1; i >= 0; i--) res = res * 10 + a[i];
        return res * sign;
    }
    friend BigInt gcd(const BigInt &a, const BigInt &b) { return b.isZero() ? a : gcd(b, a % b); }
    friend BigInt lcm(const BigInt &a, const BigInt &b) { return a / gcd(a, b) * b; }
    void read(const string &s) {
        sign = 1; a.clear(); int pos = 0;
        while (pos < int(s.size()) && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-') sign = -sign;
            pos++;
        }
        for (int i = int(s.size()) - 1; i >= pos; i--) {
            int x = 0;
            for (int j = max(pos, i); j <= i; j++) x = x * 10 + s[j] - '0';
            a.push_back(x);
        }
        trim();
    }
    friend istream& operator >> (istream &stream, BigInt &v) { string s; stream >> s; v.read(s); return stream; }
    string write() const {
        string ret = "";
        if (sign == -1) ret.push_back('-');
        ret.push_back('0' + (a.empty() ? 0 : a.back()));
        for (int i = int(a.size()) - 2; i >= 0; i--) ret.push_back('0' + a[i]);
        return ret;
    }
    friend ostream& operator << (ostream &stream, const BigInt &v) { stream << v.write(); return stream; }
    void fft(vector<complex<long double>> &a, bool invert) const {
        int n = int(a.size());
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j >= bit; bit >>= 1) j -= bit;
            j += bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            long double ang = 2 * PI / len * (invert ? -1 : 1); complex<long double> wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                complex<long double> w(1);
                for (int j = 0; j < len / 2; j++) {
                    complex<long double> u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v; a[i + j + len / 2] = u - v; w *= wlen;
                }
            }
        }
        if (invert) for (int i = 0; i < n; i++) a[i] /= n;
    }
    void multiply_fft(const vector<int> &a, const vector<int> &b, vector<int> &res) const {
        vector<complex<long double>> fa(a.begin(), a.end()), fb(b.begin(), b.end()); int n = 1;
        while (n < max(int(a.size()), int(b.size()))) n <<= 1;
        n <<= 1; fa.resize(n); fb.resize(n); fft(fa, false); fft(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        fft(fa, true); res.resize(n);
        for (int i = 0, carry = 0; i < n; i++) { res[i] = int(fa[i].real() + 0.5) + carry; carry = res[i] / 10; res[i] %= 10; }
    }
    BigInt operator * (const BigInt &v) const {
        BigInt res; res.sign = sign * v.sign; multiply_fft(a, v.a, res.a); res.trim(); return res;
    }
    BigInt mul_simple(const BigInt &v) const {
        BigInt res; res.sign = sign * v.sign; res.a.resize(int(a.size()) + int(v.a.size()));
        for (int i = 0; i < int(a.size()); i++) {
            if (a[i]) {
                for (int j = 0, carry = 0; j < int(v.a.size()) || carry; j++) {
                    long long cur = res.a[i + j] + (long long) a[i] * (j < int(v.a.size()) ? v.a[j] : 0) + carry;
                    carry = (int) (cur / 10); res.a[i + j] = (int) (cur % 10);
                }
            }
        }
        res.trim(); return res;
    }
    typedef vector<long long> vll;
    static vll karatsubaMultiply(const vll &a, const vll &b) {
        int n = int(a.size());
        if (n <= KARATSUBA_CUTOFF) {
            vll res(n << 1);
            for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) res[i + j] += a[i] * b[j];
            return res;
        }
        int k = n >> 1; vll a2(a.begin() + k, a.end()), b2(b.begin() + k, b.end()), a2b2 = karatsubaMultiply(a2, b2);
        for (int i = 0; i < k; i++) { a2[i] += a[i]; b2[i] += b[i]; }
        vll r = karatsubaMultiply(a2, b2), a1b1 = karatsubaMultiply(vll(a.begin(), a.begin() + k), vll(b.begin(), b.begin() + k)), res(n << 1);
        for (int i = 0; i < int(r.size()); i++) { res[i] += a1b1[i]; res[i + k] += r[i] - a1b1[i] - a2b2[i]; res[i + n] += a2b2[i]; }
        return res;
    }
    BigInt mul_karatsuba(const BigInt &v) const {
        vll a(this->a.begin(), this->a.end()), b(v.a.begin(), v.a.end());
        while (int(a.size()) < int(b.size())) a.push_back(0);
        while (int(b.size()) < int(a.size())) b.push_back(0);
        while (int(a.size()) & (int(a.size()) - 1)) { a.push_back(0); b.push_back(0); }
        vll c = karatsubaMultiply(a, b); BigInt res; res.sign = sign * v.sign;
        for (int i = 0, carry = 0; i < int(c.size()); i++) {
            long long cur = c[i] + carry; res.a.push_back((int) (cur % 10)); carry = (int) (cur / 10);
        }
        res.trim(); return res;
    }
};
