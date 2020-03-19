#pragma once
#include <bits/stdc++.h>
#include "../math/Combinatorics.h"
using namespace std;

seed_seq seq {
    (uint64_t)chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
mt19937_64 rng64(seq);

// Computes the hash of a string to allow for easy computation of substring hashes for H hashes
// MOD should be at least S^2 to prevent collisions
// Time Complexity:
//   compute: O(S * H)
//   getHash: O(H)
// Memory Complexity: O(S * H)
template <const int MAXN, const int HASHES, class T, const int OFFSET> struct Hashing {
    T MOD[HASHES], BASE[HASHES], H[HASHES][MAXN], POW[HASHES][MAXN];
    Hashing(const array<T, HASHES> &mod) {
        for (int h = 0; h < HASHES; h++) { MOD[h] = mod[h]; BASE[h] = uniform_int_distribution<T>(MOD[h] / 2, MOD[h] - 2)(rng64); }
    }
    void compute(const string &s) {
        int N = int(s.length());
        for (int h = 0; h < HASHES; h++) {
            POW[h][0] = 1; H[h][N] = 0;
            for (int i = 1; i <= N; i++) POW[h][i] = mulMod(POW[h][i - 1], BASE[h], MOD[h]);
            for (int i = N - 1; i >= 0; i--) H[h][i] = addMod(mulMod(H[h][i + 1], BASE[h], MOD[h]), T(s[i] - OFFSET + 1), MOD[h]);
        }
    }
    array<T, HASHES> getHash(int l, int r) { // hash for substring [l, r)
        array<T, HASHES> ret;
        for (int h = 0; h < HASHES; h++) ret[h] = subMod(H[h][l], mulMod(H[h][r], POW[h][r - l], MOD[h]), MOD[h]);
        return ret;
    }
    array<T, HASHES> concat(int l1, int r1, int l2, int r2) { // hash for the concatenated substring [l1, r1) + [l2, r2)
        array<T, HASHES> h1 = getHash(l1, r1), h2 = getHash(l2, r2), ret;
        for (int h = 0; h < HASHES; h++) addMod(h1[h], mulMod(h2[h], POW[h][r1 - l1], MOD[h]), MOD[h]);
        return ret;
    }
};
