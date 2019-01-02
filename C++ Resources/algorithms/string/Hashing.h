#pragma once
#include <bits/stdc++.h>
using namespace std;

seed_seq seq {
    (uint64_t)chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
mt19937 rng(seq);

// Computes the hash of a string to allow for easy compuptation of substring hashes
// Time Complexity:
//   compute: O(S)
//   getHash: O(1)
// Memory Complexity: O(S)
template <const int MAXN, const long long MOD> struct Hashing {
    long long H[MAXN], POW[MAXN], BASE = uniform_int_distribution<int>(MOD / 2, MOD - 1)(rng);
    void compute(const string &s) {
        int N = int(s.length()); POW[0] = 1; H[N] = 0;
        for (int i = 1; i <= N; i++) POW[i] = POW[i - 1] * BASE % MOD;
        for (int i = N - 1; i >= 0; i--) H[i] = (H[i + 1] * BASE + s[i] - 'a' + 1) % MOD;
    }
    // hash for substring [l, r)
    long long getHash(int l, int r) { return ((H[l] - H[r] * POW[r - l]) % MOD + MOD) % MOD; }
};
