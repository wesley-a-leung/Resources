#pragma once
#include <bits/stdc++.h>
using namespace std;

// Nimber operations
using T = uint64_t; constexpr const int MAXBITS = 8 * sizeof(T); T prod[MAXBITS][MAXBITS];
static_assert(is_unsigned<T>::value, "T must be an unsigned integral type");

T nimAdd(T a, T b) { return a ^ b; }

T nimMulPow2(int i, int j) { // nim product of 2^i, 2^j
    T &res = prod[i][j];
    if (res) return res;
    if (!(i & j)) return res = T(1) << (i | j);
    int a = (i & j) & -(i & j);
    return res = nimMulPow2(i ^ a, j) ^ nimMulPow2((i ^ a) | (a - 1), (j ^ a) | (i & (a - 1)));
}

T nimMul(T a, T b) {
    T res = 0;
    for (int i = 0; i < MAXBITS; i++) if ((a >> i) & 1) for (int j = 0; j < MAXBITS; j++) if ((b >> j) & 1) res ^= nimMulPow2(i, j);
    return res;
}

template <class U> T nimPow(T base, U pow) {
    T x = 1;
    for (; pow; pow /= 2, base = nimMul(base, base)) if (pow % 2) x = nimMul(x, base);
    return x;
}

T nimInv(T a) { return nimPow(a, -2); }
