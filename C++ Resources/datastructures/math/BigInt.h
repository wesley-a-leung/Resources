#pragma once
#include <bits/stdc++.h>
using namespace std;

struct BigInt {
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
            for (int i = 0, carry = 0; i < (int) max(a.size(), v.a.size()) || carry; i++) {
                if (i == (int) res.a.size()) res.a.push_back(0);
                res.a[i] += carry + (i < (int) a.size() ? a[i] : 0); carry = res.a[i] >= 10;
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
                for (int i = 0, carry = 0; i < (int) v.a.size() || carry; i++) {
                    res.a[i] -= carry + (i < (int) v.a.size() ? v.a[i] : 0); carry = res.a[i] < 0;
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
        for (int i = 0, carry = 0; i < (int) a.size() || carry; i++) {
            if (i == (int) a.size()) a.push_back(0);
            long long cur = a[i] * (long long) v + carry; carry = (int) (cur / 10); a[i] = (int) (cur % 10);
        }
        trim();
    }
    BigInt operator * (int v) const { BigInt res = *this; res *= v; return res; }
    friend pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
        int norm = 10 / (b1.a.back() + 1); BigInt a = a1.abs() * norm, b = b1.abs() * norm, q, r; q.a.resize(a.a.size());
        for (int i = a.a.size() - 1; i >= 0; i--) {
            r *= 10; r += a.a[i];
            int s1 = r.a.size() <= b.a.size() ? 0 : r.a[b.a.size()];
            int s2 = r.a.size() <= b.a.size() - 1 ? 0 : r.a[b.a.size() - 1];
            int d = ((long long) 10 * s1 + s2) / b.a.back();
            r -= b * d;
            while (r < 0) r += b, --d;
            q.a[i] = d;
        }
        q.sign = a1.sign * b1.sign; r.sign = a1.sign; q.trim(); r.trim();
        return make_pair(q, r / norm);
    }
    BigInt operator / (const BigInt &v) const { return divmod(*this, v).first; }
    BigInt operator % (const BigInt &v) const { return divmod(*this, v).second; }
    void operator /= (int v) {
        if (v < 0) sign = -sign, v = -v;
        for (int i = (int) a.size() - 1, rem = 0; i >= 0; i--) {
            long long cur = a[i] + rem * (long long) 10; a[i] = (int) (cur / v); rem = (int) (cur % v);
        }
        trim();
    }
    BigInt operator / (int v) const { BigInt res = *this; res /= v; return res; }
    int operator % (int v) const {
        if (v < 0) v = -v;
        int m = 0;
        for (int i = a.size() - 1; i >= 0; i--) m = (a[i] + m * (long long) 10) % v;
        return m * sign;
    }
    void operator += (const BigInt &v) { *this = *this + v; }
    void operator -= (const BigInt &v) { *this = *this - v; }
    void operator *= (const BigInt &v) { *this = *this * v; }
    void operator /= (const BigInt &v) { *this = *this / v; }
    bool operator < (const BigInt &v) const {
        if (sign != v.sign) return sign < v.sign;
        if (a.size() != v.a.size()) return a.size() * sign < v.a.size() * v.sign;
        for (int i = a.size() - 1; i >= 0; i--) if (a[i] != v.a[i]) return a[i] * sign < v.a[i] * sign;
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
    bool isZero() const { return a.empty() || (a.size() == 1 && !a[0]); }
    BigInt operator - () const { BigInt res = *this; res.sign = -sign; return res; }
    BigInt abs() const { BigInt res = *this; res.sign *= res.sign; return res; }
    long long longValue() const { 
        long long res = 0;
        for (int i = a.size() - 1; i >= 0; i--) res = res * 10 + a[i];
        return res * sign;
    }
    friend BigInt gcd(const BigInt &a, const BigInt &b) { return b.isZero() ? a : gcd(b, a % b); }
    friend BigInt lcm(const BigInt &a, const BigInt &b) { return a / gcd(a, b) * b; }
    void read(const string &s) {
        sign = 1; a.clear(); int pos = 0;
        while (pos < (int) s.size() && (s[pos] == '-' || s[pos] == '+')) {
            if (s[pos] == '-') sign = -sign;
            pos++;
        }
        for (int i = s.size() - 1; i >= pos; i--) {
            int x = 0;
            for (int j = max(pos, i); j <= i; j++) x = x * 10 + s[j] - '0';
            a.push_back(x);
        }
        trim();
    }
    friend istream& operator >> (istream &stream, BigInt &v) { string s; stream >> s; v.read(s); return stream; }
    friend ostream& operator << (ostream &stream, const BigInt &v) {
        if (v.sign == -1) stream << '-';
        stream << (v.a.empty() ? 0 : v.a.back());
        for (int i = (int) v.a.size() - 2; i >= 0; i--) stream << setw(1) << setfill('0') << v.a[i];
        return stream;
    }
    void fft(vector<complex<double> > & a, bool invert) const {
        int n = (int) a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j >= bit; bit >>= 1) j -= bit;
            j += bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * 3.14159265358979323846 / len * (invert ? -1 : 1); complex<double> wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                complex<double> w = (1);
                for (int j = 0; j < len / 2; j++) {
                    complex<double> u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v; a[i + j + len / 2] = u - v; w *= wlen;
                }
            }
        }
        if (invert) for (int i = 0; i < n; i++) a[i] /= n;
    }
    void multiply_fft(const vector<int> &a, const vector<int> &b, vector<int> &res) const {
        vector<complex<double>> fa(a.begin(), a.end()), fb(b.begin(), b.end()); int n = 1;
        while (n < (int) max(a.size(), b.size())) n <<= 1;
        n <<= 1; fa.resize(n); fb.resize(n); fft(fa, false); fft(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        fft(fa, true); res.resize(n);
        for (int i = 0, carry = 0; i < n; i++) {
            res[i] = int(fa[i].real() + 0.5) + carry; carry = res[i] / 10; res[i] %= 10;
        }
    }
    BigInt operator * (const BigInt &v) const {
        BigInt res; res.sign = sign * v.sign; multiply_fft(a, v.a, res.a); res.trim(); return res;
    }
    BigInt mul_simple(const BigInt &v) const {
        BigInt res; res.sign = sign * v.sign; res.a.resize(a.size() + v.a.size());
        for (int i = 0; i < (int) a.size(); i++) {
            if (a[i]) {
                for (int j = 0, carry = 0; j < (int) v.a.size() || carry; j++) {
                    long long cur = res.a[i + j] + (long long) a[i] * (j < (int) v.a.size() ? v.a[j] : 0) + carry;
                    carry = (int) (cur / 10); res.a[i + j] = (int) (cur % 10);
                }
            }
        }
        res.trim();
        return res;
    }
    typedef vector<long long> vll;
    static vll karatsubaMultiply(const vll &a, const vll &b) {
        int n = a.size(); vll res(n + n);
        if (n <= 32) {
            for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) res[i + j] += a[i] * b[j];
            return res;
        }
        int k = n >> 1;
        vll a1(a.begin(), a.begin() + k), a2(a.begin() + k, a.end()), b1(b.begin(), b.begin() + k), b2(b.begin() + k, b.end());
        vll a1b1 = karatsubaMultiply(a1, b1), a2b2 = karatsubaMultiply(a2, b2);
        for (int i = 0; i < k; i++) a2[i] += a1[i];
        for (int i = 0; i < k; i++) b2[i] += b1[i];
        vll r = karatsubaMultiply(a2, b2);
        for (int i = 0; i < (int) a1b1.size(); i++) r[i] -= a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++) r[i] -= a2b2[i];
        for (int i = 0; i < (int) r.size(); i++) res[i + k] += r[i];
        for (int i = 0; i < (int) a1b1.size(); i++) res[i] += a1b1[i];
        for (int i = 0; i < (int) a2b2.size(); i++) res[i + n] += a2b2[i];
        return res;
    }
    BigInt mul_karatsuba(const BigInt &v) const {
        vll a(this->a.begin(), this->a.end()), b(v.a.begin(), v.a.end());
        while (a.size() < b.size()) a.push_back(0);
        while (b.size() < a.size()) b.push_back(0);
        while (a.size() & (a.size() - 1)) a.push_back(0), b.push_back(0);
        vll c = karatsubaMultiply(a, b); BigInt res; res.sign = sign * v.sign;
        for (int i = 0, carry = 0; i < (int) c.size(); i++) {
            long long cur = c[i] + carry; res.a.push_back((int) (cur % 10)); carry = (int) (cur / 10);
        }
        res.trim();
        return res;
    }
};
