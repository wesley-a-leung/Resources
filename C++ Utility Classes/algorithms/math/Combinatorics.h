#ifndef ALGORITHMS_MATH_COMBINATORICS_H_
#define ALGORITHMS_MATH_COMBINATORICS_H_

#include <bits/stdc++.h>
using namespace std;

long long factorial(long long n, long long m) {
    long long ret = 1;
    for (int i = 2; i <= n; i++)
        ret = (ret * i) % m;
    return ret;
}

// O(p log n)
long long factorialPrime(long long n, long long p) {
    long long ret = 1, h = 0;
    while (n > 1) {
        ret = (ret * ((n / p) % 2 == 1 ? p - 1 : 1)) % p;
        h = n % p;
        for (int i = 2; i <= h; i++)
            ret = (ret * i) % p;
        n /= p;
    }
    return ret % p;
}

// O(log power)
long long pow3(long long base, long long pow, long long mod) {
    if (pow == 0) return 1;
    if (pow == 1) return base;
    if (pow % 2 == 0) return pow3(base * base % mod, pow / 2, mod);
    return base * pow3(base * base % mod, pow / 2, mod) % mod;
}

// O(min(k, n-k))
long long choose(int n, int k, long long p) {
    if (n < k) return 0;
    if (k > n - k) k = n - k;
    long long num = 1;
    long long den = 1;
    for (int i = 0; i < k; i++)
        num = (num * (n - i)) % p;
    for (int i = 1; i <= k; i++)
        den = (den * i) % p;
    return num * pow3(den, p - 2, p) % p;
}

long long choose(long long n, long long k) {
    if (k > n - k) k = n - k;
    long long ret = 1;
    for (int i = 0; i < k; i++)
        ret = ret * (n - i) / (i + 1);
    return ret;
}

// O(log B)
long long mulMod(long long a, long long b, long long mod) {
    long long x = 0, y = a % mod;
    for (; b > 0; b /= 2) {
        if (b % 2 == 0) x = (x + y) % mod;
        y = (y * 2) % mod;
    }
    return x;
}

// O(log power)
long long powMod(long long base, long long pow, long long mod) {
    long long x = 1, y = base % mod;
    for (; pow > 0; pow /= 2) {
        if (pow % 2 == 1) x = mulMod(x, y, mod);
        y = mulMod(y, y, mod);
    }
    return x;
}

// O(log P)
long long divMod(long long i, long long j, long long p) {
    return i * pow3(j, p - 2, p) % p;
}

// O(log P) if you precompute factorials
long long fastChoose(int n, int k, int p) {
    return divMod(divMod(factorial(n, p), factorial(k, p), p), factorial(n - k, p), p);
}

// O(k)
long long multichoose(int n, int k, long long p) {
    return choose(n + k - 1, k, p);
}

// O(k)
long long permute(int n, int k) {
    if (n < k) return 0;
    long long ret = 1;
    for (int i = 0; i < k; i++)
        ret = (ret * (n - i));
    return ret;
}

long long permute(int n, int k, long long m) {
    if (n < k) return 0;
    long long ret = 1;
    for (int i = 0; i < k; i++)
        ret = (ret * (n - i)) % m;
    return ret % m;
}

// O(n^2)
long long partitions(int n, long long m) {
    long long dp[n + 1];
    dp[0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = i; j <= n; j++)
            dp[j] = (dp[j] + dp[j - 1]) % m;
    return dp[n] % m;
}

// O(n * k)
long long partitions(int n, int k, long long m) {
    long long dp[n + 1][k + 1];
    dp[0][1] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= min(i, k); j++)
            dp[i][j] = (dp[i - 1][j - 1] + dp[i - j][j]) % m;
    return dp[n][k] % m;
}

// O(n * k)
long long stirling1(int n, int k, long long m) {
    long long dp[n + 1][k + 1];
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= k; j++) {
            dp[i][j] = ((i - 1) * dp[i - 1][j]) % m;
            dp[i][j] = (dp[i][j] + dp[i - 1][j - 1]) % m;
        }
    return dp[n][k];
}

// O(n * k)
long long stirling2(int n, int k, long long m) {
    long long dp[n + 1][k + 1];
    dp[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= k; j++) {
            dp[i][j] = (j * dp[i - 1][j]) % m;
            dp[i][j] = (dp[i][j] + dp[i - 1][j - 1]) % m;
        }
    return dp[n][k];
}

// O(n * k)
long long eulerian1(int n, int k, long long m) {
    if (k > n - 1 - k) k = n - 1 - k;
    long long dp[n + 1][k + 1];
    for (int j = 1; j <= k; j++)
        dp[0][j] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= k; j++) {
            dp[i][j] = ((i - j) * dp[i - 1][j - 1]) % m;
            dp[i][j] = (dp[i][j] + ((j + 1) * dp[i - 1][j]) % m) % m;
        }
    return dp[n][k] % m;
}

// O(n * k)
long long eulerian2(int n, int k, long long m) {
    long long dp[n + 1][k + 1];
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= k; j++) {
            if (i == j) {
                dp[i][j] = 0;
            } else {
                dp[i][j] = ((j + 1) % dp[i - 1][j]) % m;
                dp[i][j] = (((2 * i - 1 - j) * dp[i - 1][j - 1]) % m + dp[i][j]) % m;
            }
        }
    return dp[n][k] % m;
}

// O(n)
long long catalan(int n, long long p) {
    return choose(2 * n, n, p) * pow3(n + 1, p - 2, p) % p;
}

float floatPow(float base, int pow) {
    if (pow == 0) return 1.0f;
    if (pow == 1) return base;
    if (pow % 2 == 0) return floatPow(base * base, pow / 2);
    return base * floatPow(base * base, pow / 2);
}

float fastNthRoot(float x, int n) {
    if (n == 0) return 1.0f;
    float exp = 1.0f / n;
    float xExp = exp * x;
    int i = *(int*) &x; // float to int
    i = 0x3f7a3bea + exp * (i - 0x3f7a3bea);
    x = *(float*) &i; // int to float
    if (n < 0) {
        x = x * (1.0f - exp + xExp * floatPow(x, n)); // newton's approximation
        x = x * (1.0f - exp + xExp * floatPow(x, n));
        x = x * (1.0f - exp + xExp * floatPow(x, n));
    } else {
        x = x * (1.0f - exp + xExp / floatPow(x, -n)); // newton's approximation
        x = x * (1.0f - exp + xExp / floatPow(x, -n));
        x = x * (1.0f - exp + xExp / floatPow(x, -n));
    }
    return x;
}

float fastInvSqrt(float x) {
    float xhalf = 0.5f * x;
    int i = *(int*) &x;
    i = 0x5f3759df - (i >> 1);
    x = *(float*) &i;
    x = x * (1.5f - (xhalf * x * x));
    x = x * (1.5f - (xhalf * x * x));
    x = x * (1.5f - (xhalf * x * x));
    return x;
}

#endif /* ALGORITHMS_MATH_COMBINATORICS_H_ */
