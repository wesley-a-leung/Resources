#pragma once
#include <bits/stdc++.h>
using namespace std;

// Determines whether N is prime
// Time Complexity: O(sqrt N)
// Memory Complexity: O(1)
bool isPrime(long long N) {
    if (N < 2) return false;
    for (long long i = 2; i * i <= N; i++) if (N % i == 0) return false;
    return true;
}

// Returns the prime factors of N
// Time Complexity: O(sqrt N)
// Memory Complexity: O(1)
vector<long long> primeFactor(long long x) {
    vector<long long> ret;
    for (long long i = 2; i * i <= x; i++) while (x % i == 0) { ret.push_back(i); x /= i; }
    if (x > 1) ret.push_back(x);
    return ret;
}

// Returns the factors of N
// Time Complexity: O(sqrt N)
// Memory Complexity: O(1)
vector<long long> factors(long long x) {
    vector<long long> ret;
    for (long long i = 2; i * i <= x; i++) if (x % i == 0) { ret.push_back(i); if (x / i != i) ret.push_back(x / i); }
    sort(ret.begin(), ret.end());
    return ret;
}

template <class T> T powMod(T base, T pow, T mod) {
    T x = 1, y = base % mod;
    for (; pow > 0; pow /= 2, y = y * y % mod) if (pow % 2 == 1) x = x * y % mod;
    return x;
}

seed_seq seq {
    (uint64_t)chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
mt19937_64 rng64(seq);

// Determines whether N is prime using the Miller Rabin Primality Test
// Time Complexity: O(log N) * (time to square 2 integers) * iterations
// Memory Complexity: O(1)
template <class T> bool millerRabin(T N, int iterations) {
    if (N < 2 || (N != 2 && N % 2 == 0)) return false;
    T s = N - 1;
    while (s % 2 == 0) s /= 2;
    for (int i = 0; i < iterations; i++) {
        T temp = s, r = powMod(T(rng64()) % (N - 1) + 1, temp, N);
        while (temp != N - 1 && r != 1 && r != N - 1) { r = r * r % N; temp *= 2; }
        if (r != N - 1 && temp % 2 == 0) return false;
    }
    return true;
}

// Sieve of Erathosthenes to identify primes and the smallest prime factor of each number
// Time Complexity:
//   sieve: O(N)
//   primeFactor: O(log x)
// Memory Complexity: O(N)
template <const int MAXN> struct Sieve {
    bool isPrime[MAXN]; int SPF[MAXN]; vector<int> primes;
    void run(int N) {
        primes.clear(); fill(isPrime, isPrime + N + 1, true); isPrime[0] = isPrime[1] = false;
        for (int i = 2; i <= N; i++) {
            if (isPrime[i]) primes.push_back(SPF[i] = i);
            for (int j = 0; j < (int) primes.size() && i * primes[j] <= N && primes[j] <= SPF[i]; j++) {
                isPrime[i * primes[j]] = false; SPF[i * primes[j]] = primes[j];
            }
        }
    }
    vector<int> primeFactor(int x) {
        vector<int> ret;
        while (x != 1) { ret.push_back(SPF[x]); x /= SPF[x]; }
        return ret;
    }
};

// Segmented Sieve of Erathosthenes to identify primes between st and en
// Time Complexity: O(en - st)
// Memory Complexity: O(sqrt(en) + en - st)
template <const int MAXE, const int MAXRANGE> struct SegmentedSieve {
    bool sieve1[int(sqrt(MAXE)) + 5], sieve2[MAXRANGE]; vector<long long> primes;
    void run(long long st, long long en) { // [st, en] (inclusive)
        primes.clear(); st = max(st, 2LL); long long sqrtEn = sqrt(en);
        fill(sieve1, sieve1 + sqrtEn + 1, false); fill(sieve2, sieve2 + en - st + 1, false); sieve1[0] = sieve1[1] = true;
        for (long long i = 2; i <= sqrtEn; i++) {
            if (sieve1[i]) continue;
            for (long long j = i * i; j <= sqrtEn; j += i) sieve1[j] = true;
            for (long long j = (st + i - 1) / i * i; j <= en; j += i) if (j != i && !sieve2[j - st]) sieve2[j - st] = true;
        }
        for (long i = 0; i < en - st + 1; i++) if (!sieve2[i]) primes.push_back(st + i);
    }
};

// Determines the factors of all numbers from 1 to N
// Time Complexity: O(N log N)
// Memory Complexity: O(N log N)
template <const int MAXN> struct Factors {
    vector<int> factors[MAXN];
    void run(int N) { for (int i = 1; i <= N; i++) for (int j = i; j <= N; j += i) factors[j].push_back(i); }
    void clear() { for (int i = 0; i < MAXN; i++) factors[i].clear(); }
};
