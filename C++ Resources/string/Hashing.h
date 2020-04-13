#pragma once
#include <bits/stdc++.h>
#include "../math/ModularArithmetic.h"
using namespace std;

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937_64 rng64(seq);

// Computes the hash of a string to allow for easy computation of substring hashes for H hashes
// MOD should be at least S^2 to prevent collisions
// Time Complexity:
//   compute: O(S * H)
//   getHash: O(H)
// Memory Complexity: O(S * H)
template <const int MAXN, const int HASHES, class T, const int OFFSET> struct Hashing {
    T BASE[HASHES], MOD[HASHES], H[MAXN][HASHES], POW[MAXN][HASHES]; struct HASH : public array<T, HASHES> { int len; };
    Hashing(const array<T, HASHES> &mod) {
        for (int h = 0; h < HASHES; h++) { MOD[h] = mod[h]; BASE[h] = uniform_int_distribution<T>(MOD[h] / 4, MOD[h] / 4 * 3)(rng64); }
    }
    void run(const string &s) {
        int N = int(s.length()); fill(POW[0], POW[0] + HASHES, 1); fill(H[N], H[N] + HASHES, 0);
        for (int i = 1; i <= N; i++) for (int h = 0; h < HASHES; h++) POW[i][h] = mulMod(POW[i - 1][h], BASE[h], MOD[h]);
        for (int i = N - 1; i >= 0; i--) for (int h = 0; h < HASHES; h++)
            H[i][h] = addMod(mulMod(H[i + 1][h], BASE[h], MOD[h]), T(s[i] - OFFSET + 1), MOD[h]);
    }
    HASH getHash(int l, int r) { // hash for substring [l, r)
        HASH ret; ret.len = r - l; 
        for (int h = 0; h < HASHES; h++) ret[h] = subMod(H[l][h], mulMod(H[r][h], POW[r - l][h], MOD[h]), MOD[h]);
        return ret;
    }
    HASH merge(const HASH &h1, const HASH &h2) { // merges 2 hashes
        HASH ret; ret.len = h1.len + h2.len;
        for (int h = 0; h < HASHES; h++) ret[h] = addMod(h1[h], mulMod(h2[h], POW[h1.len][h], MOD[h]), MOD[h]);
        return ret;
    }
    HASH concat(int l1, int r1, int l2, int r2) { // hash for the concatenated substring [l1, r1) + [l2, r2)
        return merge(getHash(l1, r1), getHash(l2, r2));
    }
};
