#pragma once
#include <bits/stdc++.h>
using namespace std;

// a % mod, returns a value in the range [0, mod)
// Time Complexity: O(1)
template <class T> T posMod(T a, T mod) { T ret = -mod < a && a < mod ? a : a % mod; return 0 <= ret ? ret : ret + mod; }

// (a + b) % mod
// Time Complexity O(1)
// Required: 0 <= a < mod, 0 <= b < mod, mod + mod does not overflow
template <class T> T addMod(T a, T b, T mod) { T ret = a + b; return ret < mod ? ret : ret - mod; }

// (a - b) % mod
// Time Complexity O(1)
// Required: 0 <= a < mod, 0 <= b < mod, mod can be represented as a signed integer
template <class T> T subMod(T a, T b, T mod) { T ret = a - b; return 0 <= ret ? ret : ret + mod; }

// a * b % mod
// Time Complexity: O(1)
// Required: 0 <= a < mod, 0 <= b < mod, a * b does not overflow
template <class T> T mulMod(T a, T b, T mod) { return a * b % mod; }

// a * b % mod, useful if a * b overflows
// Time Complexity: O(log b)
// Required: 0 <= a < mod, 0 <= b < mod, mod + mod does not overflow
template <class T> T mulModOvf(T a, T b, T mod) {
    T x = 0;
    for (; b > 0; b >>= 1, a = addMod(a, a, mod)) if (b & 1) x = addMod(x, a, mod);
    return x;
}

struct Montgomery {
    using uint64 = uint64_t; using uint128 = __uint128_t; using int128 = __int128_t;
    struct uint256 {
        static uint128 HI(uint128 x) { return x >> 64; }
        static uint128 LO(uint128 x) { return uint64(x); }
        uint128 hi, lo;
        uint256(uint128 lo = 0) : hi(0), lo(lo) {}
        uint256(uint128 hi, uint128 lo) : hi(hi), lo(lo) {}
        static uint256 mul(uint128 x, uint128 y) {
            uint128 t1 = LO(x) * LO(y), t2 = HI(x) * LO(y) + HI(y) * LO(x) + HI(t1);
            return uint256(HI(x) * HI(y) + HI(t2), (t2 << 64) + LO(t1));
        }
    };
    uint128 mod, inv, r2;
    Montgomery(uint128 mod = 1) : mod(mod), inv(1), r2(-mod % mod) {
        for (int i = 0; i < 7; i++) inv *= 2 - mod * inv;
        for (int i = 0; i < 4; i++) if ((r2 <<= 1) >= mod) r2 -= mod;
        for (int i = 0; i < 5; i++) r2 = mul(r2, r2);
    }
    uint128 init(uint128 x) { return mul(x, r2); }
    uint128 reduce(uint256 x) {
        uint128 q = x.lo * inv; int128 a = x.hi - uint256::mul(q, mod).hi;
        return a < 0 ? a + mod : a;
    }
    uint128 mul(uint128 a, uint128 b) { return reduce(uint256::mul(a, b)); }
};

// Specialization of mulMod for unsigned 128-bit mod integers
// Uses Montgomery Reduction 
// Time Complexity: O(1)
// Required: 0 <= a < mod, 0 <= b < mod
__uint128_t mulMod128(__uint128_t a, __uint128_t b, __uint128_t mod) {
    static Montgomery mont;
    if (mont.mod != mod) mont = Montgomery(mod);
    return mont.reduce(mont.mul(mont.init(a), mont.init(b)));
}

// base ^ pow % mod
// Time Complexity: O(log pow)
// If multiplication is an expensive operation, then y = y * y should only be computed when pow > 0
// Required: 0 <= base < mod, 0 <= pow, 0 < mod, mod * mod does not overflow
template <class T, class U> T powMod(T base, U pow, T mod) {
    T x = 1;
    for (; pow > 0; pow >>= 1, base = mulMod(base, base, mod)) if (pow & 1) x = mulMod(x, base, mod);
    return x;
}

// base ^ pow % mod, useful when mod * mod overflows
// Time Complexity: O(log pow)
// If multiplication is an expensive operation, then y = y * y should only be computed when pow > 0
// Required: 0 <= base < mod, 0 <= pow, 0 < mod, mod + mod does not overflow
template <class T, class U> T powModOvf(T base, U pow, T mod) {
    T x = 1;
    for (; pow > 0; pow >>= 1, base = mulModOvf(base, base, mod)) if (pow & 1) x = mulModOvf(x, base, mod);
    return x;
}

// Modular Multiplicative Inverse of i in Zp for a prime p
// Time Complexity: O(log p)
// Required: 0 < i < p, p * p does not overflow
template <class T> T mulInvPrime(T i, T p) { return powMod(i, p - 2, p); }

// i / j % p for a prime p
// Time Complexity: O(log p)
// Required: 0 <= i < p, 0 < j < p, p * p does not overflow
template <class T> T divModPrime(T i, T j, T p) { return mulMod(i, mulInvPrime(j, p), p); }
