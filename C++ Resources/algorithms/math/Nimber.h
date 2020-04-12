#pragma once
#include <bits/stdc++.h>
using namespace std;

// Nimber operations
using T = uint64_t; constexpr const int BITS = 8 * sizeof(T); T prod[BITS][BITS];
static_assert(is_unsigned<T>::value, "T must be an unsigned integral type");

T addNim(T a, T b) { return a ^ b; }

T mulNimPow2(int i, int j) { // nim product of 2^i, 2^j
    T &res = prod[i][j];
    if (res) return res;
    if (!(i & j)) return res = T(1) << (i | j);
    int a = (i & j) & -(i & j);
    return res = mulNimPow2(i ^ a, j) ^ mulNimPow2((i ^ a) | (a - 1), (j ^ a) | (i & (a - 1)));
}

T mulNim(T a, T b) {
    T res = 0;
    for (int i = 0; i < BITS; i++) if ((a >> i) & 1) for (int j = 0; j < BITS; j++) if ((b >> j) & 1) res = addNim(res, mulNimPow2(i, j));
    return res;
}

template <class U> T powNim(T base, U pow) {
    T x = 1;
    for (; pow; pow /= 2, base = mulNim(base, base)) if (pow % 2) x = mulNim(x, base);
    return x;
}

T mulInvNim(T a) { return powNim(a, -2); }
