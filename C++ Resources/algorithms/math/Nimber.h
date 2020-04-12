#pragma once
#include <bits/stdc++.h>
using namespace std;

// Nimber operations
using u64 = uint64_t; const int MAXBITS = 64; u64 prod[MAXBITS][MAXBITS];

u64 nimAdd(u64 a, u64 b) { return a ^ b; }

u64 nimMulPow2(int i, int j) { // nim product of 2^i, 2^j
    u64 &res = prod[i][j];
    if (res) return res;
    if (!(i & j)) return res = u64(1) << (i | j);
    int a = (i & j) & -(i & j);
    return res = nimMulPow2(i ^ a, j) ^ nimMulPow2((i ^ a) | (a - 1), (j ^ a) | (i & (a - 1)));
}

u64 nimMul(u64 a, u64 b) {
    u64 res = 0;
    for (int i = 0; i < MAXBITS; i++) if ((a >> i) & 1) for (int j = 0; j < MAXBITS; j++) if ((b >> j) & 1) res ^= nimMulPow2(i, j);
    return res;
}

template <class T> u64 nimPow(u64 base, T pow) {
    u64 x = 1;
    for (; pow; pow /= 2, base = nimMul(base, base)) if (pow % 2) x = nimMul(x, base);
    return x;
}

u64 nimInv(u64 a) { return nimPow(a, -2); }
