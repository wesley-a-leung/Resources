#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class T, const T MOD> struct IntMod {
    static_assert(is_integral<T>::value, "T must be an integral type");
    static_assert(0 < MOD, "MOD must be a positive integer");
    using IM = IntMod<T, MOD>; T v = 0; IntMod() {}
    IntMod(const T &x) { v = -MOD < x && x < MOD ? x : x % MOD; if (v < 0) v += MOD; }
    void operator = (const T &x) { v = -MOD < x && x < MOD ? x : x % MOD; if (v < 0) v += MOD; }
    bool operator < (const IM &i) const { return v < i.v; }
    bool operator <= (const IM &i) const { return v <= i.v; }
    bool operator > (const IM &i) const { return v > i.v; }
    bool operator >= (const IM &i) const { return v >= i.v; }
    bool operator == (const IM &i) const { return v == i.v; }
    bool operator != (const IM &i) const { return v != i.v; }
    IM operator ++ () { if (++v == MOD) v = 0; return *this; }
    IM operator ++ (int) { IM ret = *this; if (++v == MOD) v = 0; return ret; }
    IM operator -- () { if (v-- == 0) v = MOD - 1; return *this; }
    IM operator -- (int) { IM ret = *this; if (v-- == 0) v = MOD - 1; return ret; }
    IM operator + (const IM &i) const { IM ret; ret.v = v + i.v; if (ret.v >= MOD) ret.v -= MOD; return ret;  }
    IM &operator += (const IM &i) { v += i.v; if (v >= MOD) v -= MOD; return *this; }
    IM operator - (const IM &i) const { IM ret; ret.v = v - i.v; if (ret.v < 0) ret.v += MOD; return ret;  }
    IM &operator -= (const IM &i) { v -= i.v; if (v < 0) v += MOD; return *this; }
    IM operator * (const IM &i) const { return IM(v * i.v % MOD); }
    IM &operator *= (const IM &i) { v = v * i.v % MOD; return *this; }
    // when MOD * MOD overflows
    IM mulOvf(const IM &i) const {
        IM x = 0, y = *this; T z = i.v;
        for (; z > 0; z >>= 1, y += y) if (z & 1) x += y;
        return x;
    }
    // when MOD * MOD doesn't overflows
    template <class U> IM pow(U p) const {
        IM x = 1, y = *this;
        for (; p > 0; p >>= 1, y *= y) if (p & 1) x *= y;
        return x;
    }
    // when MOD * MOD overflows
    template <class U> IM powOvf(U p) const {
        IM x = 1, y = *this;
        for (; p > 0; p >>= 1, y = y.mulOvf(y)) if (p & 1) x = x.mulOvf(y);
        return x;
    }
    static T EEA(T a, T b, T &x, T &y) {
        if (b == 0) { x = 1; y = 0; return a; }
        T x1, y1, g = EEA(b, a % b, x1, y1); y = x1 - (a / b) * y1; x = y1; return g;
    }
    IM mulInv() const {
        T x, y;
        if (EEA(v, MOD, x, y) != 1) throw 0;
        return IM(x);
    }
    // for prime MOD, MOD * MOD doesn't overflow
    IM mulInvPrime() const { return pow(MOD - 2); }
    IM operator / (const IM &i) const { return *this * i.mulInv(); }
    IM &operator /= (const IM &i) { return *this *= i.mulInv(); }
    // for prime MOD, MOD * MOD doesn't overflow
    IM divPrime(const IM &i) const { return *this * i.mulInvPrime(); }
    // if MOD * MOD overflows
    IM divOvf(const IM &i) const { return mulOvf(i.mulInv()); }
    friend istream &operator >> (istream &stream, IM &v) { stream >> v; return stream; }
    friend ostream &operator << (ostream &stream, IM &v) { stream << v; return stream; }
};
