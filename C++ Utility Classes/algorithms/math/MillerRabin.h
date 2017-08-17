#ifndef ALGORITHMS_MATH_MILLERRABIN_H_
#define ALGORITHMS_MATH_MILLERRABIN_H_

#include <bits/stdc++.h>

using namespace std;

unsigned long long mulMod(unsigned long long a, unsigned long long b, unsigned long long mod) {
    unsigned long long x = 0, y = a % mod;
    for (; b > 0; b /= 2) {
        if (b % 2 == 1) x = (x + y) % mod;
        y = (y * 2) % mod;
    }
    return x % mod;
}

unsigned long long powMod(unsigned long long base, unsigned long long pow, unsigned long long mod) {
    unsigned long long x = 1, y = base;
    for (; pow > 0; pow /= 2) {
        if (pow % 2 == 1) x = mulMod(x, y, mod);
        y = mulMod(y, y, mod);
    }
    return x % mod;
}

bool isPrime(unsigned long long n, int iterations) {
    if (n < 2 || (n != 2 && n % 2 == 0)) return false;
    unsigned long long s = n - 1;
    while (s % 2 == 0) s /= 2;
    srand(time(0));
    for (int i = 0; i < iterations; i++) {
        unsigned long long temp = s;
        unsigned long long r = powMod(rand(), temp, n);
        while (temp != n - 1 && r != 1 && r != n - 1) {
            r = mulMod(r, r, n);
            temp *= 2;
        }
        if (r != n - 1 && temp % 2 == 0) return false;
    }
    return true;
}

#endif /* ALGORITHMS_MATH_MILLERRABIN_H_ */
