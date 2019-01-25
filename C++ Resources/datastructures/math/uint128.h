#pragma once
#include <bits/stdc++.h>
using namespace std;

using uint64 = uint64_t; 
void mult64to128(uint64 u, uint64 v, uint64 &h, uint64 &l);void mult64to128(uint64 u, uint64 v, uint64 &h, uint64 &l);
void divmod128by64(uint64 u1, uint64 u0, uint64 v, uint64 &q, uint64 &r);

struct uint128 {
    uint64 hi, lo;
    uint128(const uint64 &lo = 0) : hi(0), lo(lo) {}
    uint128(const uint64 &hi, const uint64 &lo) : hi(hi), lo(lo) {}
    void operator = (const uint64 &v) { hi = 0; lo = v; }
    bool operator < (const uint128 &v) const { return hi < v.hi || (hi == v.hi && lo < v.lo); }
    bool operator <= (const uint128 &v) const { return hi < v.hi || (hi == v.hi && lo <= v.lo); }
    bool operator > (const uint128 &v) const { return hi > v.hi || (hi == v.hi && lo > v.lo); }
    bool operator >= (const uint128 &v) const { return hi > v.hi || (hi == v.hi && lo >= v.lo); }
    bool operator == (const uint128 &v) const { return hi == v.hi && lo == v.lo; }
    bool operator != (const uint128 &v) const { return hi != v.hi || lo != v.lo; }
    uint128 operator ~ () const { return uint128(~hi, ~lo); }
    uint128 operator | (const uint128 &v) const { return uint128(hi | v.hi, lo | v.lo); }
    uint128 &operator |= (const uint128 &v) { hi |= v.hi; lo |= v.lo; return *this; }
    uint128 operator & (const uint128 &v) const { return uint128(hi & v.hi, lo & v.lo);  }
    uint128 &operator &= (const uint128 &v) { hi &= v.hi; lo &= v.lo; return *this; }
    uint128 operator ^ (const uint128 &v) const { return uint128(hi ^ v.hi, lo ^ v.lo); }
    uint128 &operator ^= (const uint128 &v) { hi ^= v.hi; lo ^= v.lo; return *this; }
    uint128 operator << (uint32_t v) const {
        v &= 127; uint64 m1 = ((((v + 127) | v) & 64) >> 6) - 1llu, m2 = (v >> 6) - 1llu; v &= 63;
        return uint128(((lo << v) & (~m2)) | (((hi << v) | ((lo >> (64 - v)) & m1)) & m2), (lo << v) & m2);
    }
    uint128 &operator <<= (uint32_t v) { return *this = *this << v; }
    uint128 operator >> (uint32_t v) const {
        v &= 127; uint64 m1 = ((((v + 127) | v) & 64) >> 6) - 1llu, m2 = (v >> 6) - 1llu; v &= 63;
        return uint128((hi >> v) & m2, ((hi >> v) & (~m2)) | (((lo >> v) | ((hi << (64 - v)) & m1)) & m2));
    }
    uint128 &operator >>= (uint32_t v) { return *this = *this >> v; }
    friend int popcnt128(const uint128 &v);
    friend int clz128(const uint128 &v);
    friend int ctz128(const uint128 &v);
    uint128 operator ++ () { uint64 t = lo + 1; hi += ((lo ^ t) & lo) >> 63; lo = t; return *this; }
    uint128 operator ++ (int) { uint128 ret = *this; uint64 t = lo + 1; hi += ((lo ^ t) & lo) >> 63; lo = t; return ret; }
    uint128 operator -- () { uint64 t = lo - 1; hi -= ((t ^ lo) & t) >> 63; lo = t; return *this; }
    uint128 operator -- (int) { uint128 ret = *this; uint64 t = lo - 1; hi -= ((t ^ lo) & t) >> 63; lo = t; return ret; }
    uint128 operator + (const uint128 &v) const { return uint128(hi + v.hi + ((((lo & v.lo) & 1) + (lo >> 1) + (v.lo >> 1)) >> 63), lo + v.lo); }
    uint128 &operator += (const uint128 &v) { hi += v.hi + ((((lo & v.lo) & 1) + (lo >> 1) + (v.lo >> 1)) >> 63); lo += v.lo; return *this; }
    uint128 operator - (const uint128 &v) const { uint64 t = lo - v.lo; return uint128(hi - (v.hi + ((((t & v.lo) & 1) + (v.lo >> 1) + (t >> 1)) >> 63)), t); }
    uint128 &operator -= (const uint128 &v) { uint64 t = lo - v.lo; hi -= (v.hi + ((((t & v.lo) & 1) + (v.lo >> 1) + (t >> 1)) >> 63)); lo = t; return *this; }
    uint128 operator * (const uint128 &v) const { uint128 ret; mult64to128(lo, v.lo, ret.hi, ret.lo); ret.hi += (hi * v.lo) + (lo * v.hi); return ret; }
    uint128 &operator *= (const uint128 &v) { return *this = *this * v; }
    friend void mult128to256(uint128 u, uint128 v, uint128 &h, uint128 &l);
    friend void bindivmod128(uint128 u, uint128 v, uint128 &q, uint128 &r);
    friend void divmod128by128(uint128 u, uint128 v, uint128 &q, uint128 &r);
    friend void divmod128(uint128 u, uint128 v, uint128 &q, uint128 &r);
    uint128 operator / (const uint128 &v) const { uint128 q, r; divmod128(*this, v, q, r); return q; }
    uint128 &operator /= (const uint128 &v) { return *this = *this / v; }
    uint128 operator % (const uint128 &v) const { uint128 q, r; divmod128(*this, v, q, r); return r; }
    uint128 &operator %= (const uint128 &v) { return *this = *this % v; }
};

int popcnt128(const uint128 &v) { return __builtin_popcountll(v.hi) + __builtin_popcountll(v.lo); }
int clz128(const uint128 &v) { return v.hi == 0 ? __builtin_clzll(v.lo) + 64 : __builtin_clzll(v.hi); }
int ctz128(const uint128 &v) { return v.lo == 0 ? __builtin_ctzll(v.hi) + 64 : __builtin_ctzll(v.lo); }
void mult64to128(uint64 u, uint64 v, uint64 &h, uint64 &l) {
    uint64 u1 = u & 0xffffffff, v1 = v & 0xffffffff, t = u1 * v1, w3 = t & 0xffffffff, k = t >> 32; u >>= 32; t = (u * v1) + k; k = (t & 0xffffffff);
    uint64 w1 = (t >> 32); v >>= 32; t = (u1 * v) + k; k = (t >> 32); h = (u * v) + w1 + k; l = (t << 32) + w3;
}
void mult128to256(uint128 u, uint128 v, uint128 &h, uint128 &l) {
    mult64to128(u.hi, v.hi, h.hi, h.lo); mult64to128(u.lo, v.lo, l.hi, l.lo); uint128 t; mult64to128(u.hi, v.lo, t.hi, t.lo);
    if ((l.hi += t.lo) < t.lo) ++h;
    if ((h.lo += t.hi) < t.hi) ++h.hi;
    mult64to128(u.lo, v.hi, t.hi, t.lo);
    if ((l.hi += t.lo) < t.lo) ++h;
    if ((h.lo += t.hi) < t.hi) ++h.hi;
}
void bindivmod128(uint128 u, uint128 v, uint128 &q, uint128 &r) {
    q.hi = q.lo = 0; int shift = clz128(v) - clz128(u); v <<= shift;
    do {
        q <<= 1;
        if (v <= u) { u -= v; q.lo |= 1; }
        v >>= 1;
    } while (shift-- != 0);
    r.hi = u.hi; r.lo = u.lo;
}
void divmod128by64(uint64 u1, uint64 u0, uint64 v, uint64 &q, uint64 &r) {
    const uint64 b = 1ll << 32; uint64 un1, un0, vn1, vn0, q1, q0, un32, un21, un10, rhat, left, right;
    int s = __builtin_clzll(v); v <<= s; vn1 = v >> 32; vn0 = v & 0xffffffff;
    if (s > 0) { un32 = (u1 << s) | (u0 >> (64 - s)); un10 = u0 << s; }
    else { un32 = u1; un10 = u0; }
    un1 = un10 >> 32; un0 = un10 & 0xffffffff; q1 = un32 / vn1; rhat = un32 % vn1; left = q1 * vn0; right = (rhat << 32) + un1;
label1:
    if ((q1 >= b) || (left > right)) {
        --q1; rhat += vn1;
        if (rhat < b) { left -= vn0; right = (rhat << 32) | un1; goto label1; }
    }
    un21 = (un32 << 32) + (un1 - (q1 * v)); q0 = un21 / vn1; rhat = un21 % vn1; left = q0 * vn0; right = (rhat << 32) | un0;
label2:
    if ((q0 >= b) || (left > right)) {
        --q0; rhat += vn1;
        if (rhat < b) { left -= vn0; right = (rhat << 32) | un0; goto label2; }
    }
    r = ((un21 << 32) + (un0 - (q0 * v))) >> s; q = (q1 << 32) | q0;
}
void divmod128by128(uint128 u, uint128 v, uint128 &q, uint128 &r) {
    if (v.hi == 0) {
        if (u.hi < v.lo) { divmod128by64(u.hi, u.lo, v.lo, q.lo, r.lo); q.hi = 0; r.hi = 0; return; }
        else { q.hi = u.hi / v.lo; r.hi = u.hi % v.lo; divmod128by64(r.hi, u.lo, v.lo, q.lo, r.lo); r.hi = 0; return; }
    } else {
        int n = __builtin_clzll(v.hi); uint128 v1 = v << n, u1 = u >> 1, q1; uint64 r1;
        divmod128by64(u1.hi, u1.lo, v1.hi, q1.lo, r1); q1.hi = 0; q1 >>= 63 - n;
        if ((q1.hi | q1.lo) != 0) --q1;
        q.hi = q1.hi; q.lo = q1.lo; q1 *= v; r = u - q1;
        if (v <= r) { ++q; r -= v; }
        return;
    }
}
void divmod128(uint128 u, uint128 v, uint128 &q, uint128 &r) {
    int ulz = clz128(u), vlz = clz128(v), vtz = ctz128(v);
    if (vlz == 128) throw 0;
    else if ((u.hi | v.hi) == 0) { q.hi = r.hi = 0; q.lo = u.lo / v.lo; r.lo = u.lo % v.lo; return; }
    else if (vlz == 127) { q = u; r.hi = r.lo = 0; return; }
    else if ((vtz + vlz) == 127) { q = u >> vtz; r = (--v) & u; return; }
    if (u < v) { q.hi = q.lo = 0; r = u; return; }
    else if (u == v) { q.hi = r.hi = r.lo = 0; q.lo = 1; return; }
    if ((vlz - ulz) > 5) divmod128by128(u, v, q, r);
    else bindivmod128(u, v, q, r);
}
