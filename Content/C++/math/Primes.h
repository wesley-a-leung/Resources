#pragma once
#include <bits/stdc++.h>
#include "ModularArithmetic.h"
#include "GCD.h"
#if __cplusplus < 201402L
#include "../utils/MakeUnique.h"
#endif
using namespace std;

// Determines whether x is prime
// Time Complexity: O(sqrt x)
// Memory Complexity: O(1)
bool isPrime(long long x) {
    if (x < 2) return false;
    for (long long i = 2; i * i <= x; i++) if (x % i == 0) return false;
    return true;
}

// Returns the prime factors of x
// Time Complexity: O(sqrt x)
// Memory Complexity: O(log x)
vector<long long> primeFactor(long long x) {
    vector<long long> ret;
    for (long long i = 2; i * i <= x; i++) while (x % i == 0) { ret.push_back(i); x /= i; }
    if (x > 1) ret.push_back(x);
    return ret;
}

// Returns the prime factors of x and the count of each factor
// Time Complexity: O(sqrt x)
// Memory Complexity: O(log x)
vector<pair<long long, int>> primeFactorWithCount(long long x) {
    vector<pair<long long, int>> ret;
    for (long long i = 2; i * i <= x; i++) if (x % i == 0) for (ret.emplace_back(i, 0); x % i == 0; x /= i) ret.back().second++;
    if (x > 1) ret.emplace_back(x, 1);
    return ret;
}

// Returns the factors of x
// Time Complexity: O(sqrt x)
// Memory Complexity: O(sqrt x)
vector<long long> factors(long long x) {
    vector<long long> ret;
    for (long long i = 1; i * i <= x; i++) if (x % i == 0) { ret.push_back(i); if (x / i != i) ret.push_back(x / i); }
    return ret;
}

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937_64 rng64(seq);

// Determines whether N is prime using the Miller Rabin Primality Test
// Time Complexity: iterations * time complexity of powMod
// Memory Complexity: O(iterations)
template <class T> bool millerRabin(T N, int iterations = 7) {
    if (N < 2 || N % 6 % 4 != 1) return (N | 1) == 3;
    vector<T> A = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    while (int(A.size()) < iterations) A.push_back(uniform_int_distribution<long long>(1795265023, numeric_limits<long long>::max())(rng64));
    int s = 0;
    while (!(((N - 1) >> s) & 1)) s++;
    T d = N >> s;
    for (T a : A) {
        T p = powMod(a % N, d, N); int i = s;
        while (p != 1 && p != N - 1 && a % N != 0 && i--) p = mulMod(p, p, N);
        if (p != N - 1 && i != s) return false;
    }
    return true;
}

// Sieve of Erathosthenes to identify primes and the smallest prime factor of each number
// Time Complexity:
//   run: O(N)
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
    vector<pair<int, int>> primeFactorWithCount(int x) {
        vector<pair<int, int>> ret;
        while (x != 1) {
            ret.emplace_back(SPF[x], 0);
            for (int spf = SPF[x]; x % spf == 0; x /= spf) ret.back().second++;
        }
        return ret;
    }
};

// Counts the number of positive integers less than or equal to x that are relatively prime to x
// Time Complexity: O(sqrt x)
long long phi(long long x) {
    long long ret = x;
    for (long long i = 2; i * i <= x; i++) if (x % i == 0) for (ret -= ret / i; x % i == 0; x /= i);
    if (x > 1) ret -= ret / x;
    return ret;
}

// Euler's Totient function
// phi[i] = number of positive integers less than or equal to i that are relatively prime to i
// Time Complexity:
//   run: O(N log N)
// Memory Complexity: O(N)
template <const int MAXN> struct EulersTotient {
    int phi[MAXN]; 
    void run(int N) {
        iota(phi, phi + N + 1, 0);
        for (int i = 2; i <= N; i++) if (phi[i] == i) {
            phi[i] = i - 1;
            for (int j = 2 * i; j <= N; j += i) phi[j] = phi[j] / i * (i - 1);
        }
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


// Returns a divisor of N
// Time Complexity: O(log N)
// Memory Complexity: O(1)
template <class T> T pollardsRho(T N, int iterations = 40) {
    auto f = [&] (T x) { return mulMod(x, x, N) + 1; };
    T x = 0, y = 0, p = 2, q; int t = 0, i = 1;
    while (t++ % iterations != 0 || gcd(p, N) == 1) {
        if (x == y) y = f(x = ++i);
        if ((q = mulMod(p, max(x, y) - min(x, y), N)) != 0) p = q;
        x = f(x); y = f(f(y));
    }
    return gcd(p, N);
}

// Returns the prime factors of N
// Time Complexity: O(x ^ (1/4) * (log x))
// Memory Complexity: O(log x)
template <class T> vector<T> pollardsRhoPrimeFactor(T x, int pollardsRhoIters = 40, int millerRabinIters = 7) {
    vector<T> ret; queue<T> q; q.push(x);
    while (!q.empty()) {
        T y = q.front(); q.pop();
        if (millerRabin(y, millerRabinIters)) ret.push_back(y);
        else { q.push(pollardsRho(y, pollardsRhoIters)); q.push(y / q.back()); }
    }
    return ret;
}

// Returns the prime factors of N and the count of each factor
// Time Complexity: O(x ^ (1/4) * (log x) ^ 2)
// Memory Complexity: O(log x)
template <class T> vector<pair<T, int>> pollardsRhoPrimeFactorWithCount(T x, int pollardsRhoIters = 40, int millerRabinIters = 7) {
    vector<T> pf = pollardsRhoPrimeFactor(x, pollardsRhoIters, millerRabinIters); sort(pf.begin(), pf.end()); vector<pair<T, int>> ret;
    for (int i = 0, cnt = 0; i < int(pf.size()); i++) {
        cnt++;
        if (i + 1 == int(pf.size()) || pf[i] != pf[i + 1]) { ret.emplace_back(pf[i], cnt); cnt = 0; }
    }
    return ret;
}

// Determines the factors of all numbers from 1 to N
// Time Complexity: O(N log N)
// Memory Complexity: O(N log N)
template <const int MAXN> struct Factors {
    vector<int> factors[MAXN];
    void run(int N) { for (int i = 1; i <= N; i++) for (int j = i; j <= N; j += i) factors[j].push_back(i); }
    void clear() { for (int i = 0; i < MAXN; i++) factors[i].clear(); }
};
