#pragma once
#include <bits/stdc++.h>
using namespace std;

// FenwickTree supporting range updates with updates in the form of
// adding v, 2^k * v, 3^k * v, ... to the interval [l, r], and range queries
// Time Complexity:
//   constructor: O(N + K^3)
//   update: O(K (K + log N))
//   rsq: O(K log N)
// Memory Complexity: O(KN + K^3)
template <class T> struct FenwickTreePolynomial {
    T powMod(T base, T pow, T mod) {
        T x = 1, y = base % mod;
        for (; pow > 0; pow /= 2, y = y * y % mod) if (pow % 2 == 1) x = x * y % mod;
        return x;
    }
    int size, maxK; T mod;
    vector<vector<T>> BIT;
    vector<vector<T>> cof {{0, 1},
                           {0, 1, 1},
                           {0, 1, 3, 2},
                           {0, 0, 6, 12, 6},
                           {0, -4, 0, 40, 60, 24},
                           {0, 0, -60, 0, 300, 360, 120},
                           {0, 120, 0, -840, 0, 2520, 2520, 720},
                           {0, 0, 3360, 0, -11760, 0, 23520, 20160, 5040},
                           {0, -12096, 0, 80640, 0, -169344, 0, 241920, 181440, 40320},
                           {0, 0, -544320, 0, 1814400, 0, -2540160, 0, 2721600, 1814400, 362880},
                           {0, 3024000, 0, -19958400, 0, 39916800, 0, -39916800, 0, 33264000, 19958400, 3628800}};
    vector<vector<vector<T>>> sumDiff;
    vector<T> factorial;
    vector<vector<T>> pascal;
    T invDen;
    T rsq(vector<T> &BIT, int i) { T ret = 0; for (; i > 0; i -= i & -i) (ret += BIT[i]) %= mod; return ret; }
    void update(vector<T> &BIT, int i, T v) { for (; i <= size; i += i & -i) (BIT[i] += v) %= mod; }
    FenwickTreePolynomial(int size, int maxK, T modbigPrime) : size(size), maxK(maxK), mod(modbigPrime), BIT(maxK + 2), sumDiff(maxK + 1), pascal(maxK + 2) {
        assert(maxK >= 0 && maxK <= 10);
        factorial.push_back(1);
        for (int i = 1; i < maxK + 2; i++) factorial.push_back(factorial.back() * i);
        for (int i = 0; i < maxK + 2; i++) for (int j = 0; j <= size; j++) BIT[i].push_back(0);
        for (int k = 0; k < maxK + 1; k++) {
            for (int i = 0; i < maxK + 2; i++) {
                sumDiff[k].push_back(vector<T>());
                for (int j = 0; j < maxK + 2 - i; j++) sumDiff[k][i].push_back(0);
            }
        }
        for (int i = 0; i < maxK + 2; i++) {
            pascal[i].push_back(1);
            for (int j = 1; j <= i; j++) pascal[i].push_back(pascal[i].back() * (i - j + 1) / j);
        }
        for (int k = 0; k < maxK + 1; k++) {
            for (int i = 0; i < k + 2; i++) {
                int sign = 1;
                for (int j = 0; j <= i; j++) {
                    sumDiff[k][i - j][j] = (sumDiff[k][i - j][j] + (((cof[k][i] * pascal[i][j]) % mod * (factorial[maxK + 1] / factorial[k + 1])) % mod) * sign) % mod;
                    sign = -sign;
                }
            }
        }
        invDen = powMod(factorial[maxK + 1], mod - 2, mod);
    }
    T rsq(int ind) {
        T sum = rsq(BIT[maxK + 1], ind);
        for (int i = maxK; i >= 0; i--) sum = ((sum * ind) % mod + rsq(BIT[i], ind)) % mod;
        if (sum < 0) sum += mod;
        return sum * invDen % mod;
    }
    T rsq(int a, int b) {
        T sum = (rsq(b) - rsq(a - 1)) % mod;
        if (sum < 0) sum += mod;
        return sum;
    }
    void update(int a, int b, T value, int k) {
        value %= mod;
        if (value < 0) value += mod;
        T s = a - 1, len = b - a + 1, mult;
        for (int i = 0; i < k + 2; i++) {
            mult = sumDiff[k][i][k + 1 - i];
            for (int j = k - i; j >= 0; j--) mult = ((mult * s) % mod + sumDiff[k][i][j]) % mod;
            update(BIT[i], a, value * mult % mod);
            update(BIT[i], b + 1, -value * mult % mod);
        }
        mult = cof[k][k + 1];
        for (int i = k; i >= 0; i--) mult = ((mult * len) % mod + cof[k][i]) % mod;
        if (mult < 0) mult += mod;
        mult = (mult * (factorial[maxK + 1] / factorial[k + 1])) % mod;
        update(BIT[0], b + 1, value * mult % mod);
    }
};
