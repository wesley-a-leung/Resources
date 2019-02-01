#pragma once
#include <bits/stdc++.h>
using namespace std;

seed_seq seq {
    (uint64_t) chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
    (uint64_t) __builtin_ia32_rdtsc(),
    (uint64_t) (uintptr_t) make_unique<char>().get()
};
mt19937_64 rng64(seq);

// Computes the hash of a string to allow for easy compuptation of substring hashes
// Time Complexity:
//   compute: O(S)
//   getHash: O(1)
// Memory Complexity: O(S)
template <const int MAXN, class T, const T MOD> struct Hashing {
    T H[MAXN], POW[MAXN], BASE = uniform_int_distribution<T>(MOD / 2, MOD - 1)(rng64);
    void compute(const string &s, T base = -1) {
        if (base == -1) base = BASE;
        int N = int(s.length()); POW[0] = 1; H[N] = 0;
        for (int i = 1; i <= N; i++) POW[i] = POW[i - 1] * base % MOD;
        for (int i = N - 1; i >= 0; i--) H[i] = (H[i + 1] * base + s[i] - 'a' + 1) % MOD;
    }
    // hash for substring [l, r)
    T getHash(int l, int r) { return ((H[l] - H[r] * POW[r - l]) % MOD + MOD) % MOD; }
};
