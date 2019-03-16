#pragma once
#include <bits/stdc++.h>
using namespace std;

template <class T, const T MOD> struct IntMod {
    // improves the performance of mod on x86 machines
    static uint32_t fastMod(uint64_t x, uint32_t m) {
    #if !defined(_WIN32) || defined(_WIN64)
        return x % m;
    #endif
        uint32_t hi = x >> 32, lo = x, q, r; assert(hi < m); asm("divl %4\n" : "=a" (q), "=d" (r) : "d" (hi), "a" (lo), "r" (m)); return r;
    }
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
    // when MOD * MOD doesn't overflow
    IM operator * (const IM &i) const { return IM(fastMod(v * i.v, MOD)); }
    // when MOD * MOD doesn't overflow
    IM &operator *= (const IM &i) { v = fastMod(v * i.v, MOD); return *this; }
    // when MOD * MOD overflows
    IM mulOvf(const IM &i) const {
        IM x = 0, y = *this; T z = i.v;
        for (; z > 0; z >>= 1, y += y) if (z & 1) x += y;
        return x;
    }
    // when MOD * MOD doesn't overflow
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
    IM mulInv() const {
        T g = MOD, r = v, x = 0, y = 1;
        while (r != 0) { T q = g / r; g %= r; swap(g, r); x -= q * y; swap(x, y); }
        assert(g == 1); return IM(x);
    }
    // for prime MOD, MOD * MOD doesn't overflow
    IM mulInvPrime() const { return pow(MOD - 2); }
    // when MOD * MOD doesn't overflow
    IM operator / (const IM &i) const { return *this * i.mulInv(); }
    // when MOD * MOD doesn't overflow
    IM &operator /= (const IM &i) { return *this *= i.mulInv(); }
    // for prime MOD, MOD * MOD doesn't overflow
    IM divPrime(const IM &i) const { return *this * i.mulInvPrime(); }
    // if MOD * MOD overflows
    IM divOvf(const IM &i) const { return mulOvf(i.mulInv()); }
    friend istream &operator >> (istream &stream, IM &v) { stream >> v; return stream; }
    friend ostream &operator << (ostream &stream, IM &v) { stream << v; return stream; }
};
