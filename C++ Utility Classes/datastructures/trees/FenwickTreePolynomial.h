#ifndef DATASTRUCTURES_TREES_FENWICKTREEPOLYNOMIAL_H_
#define DATASTRUCTURES_TREES_FENWICKTREEPOLYNOMIAL_H_

#include <bits/stdc++.h>
using namespace std;

long long pow3(long long base, long long pow, long long mod) {
    if (pow == 0) return 1;
    if (pow == 1) return base;
    if (pow % 2 == 0) return pow3(base * base % mod, pow / 2, mod);
    return base * pow3(base * base % mod, pow / 2, mod) % mod;
}

long long divMod(long long i, long long j, long long p) {
    return i * pow3(j, p - 2, p) % p;
}

/**
 * FenwickTree supporting range updates with updates in the form of
 * adding v, 2^k * v, 3^k * v, ... to the interval [l, r], and range queries.
 * Memory usage:  O(kn)
 */
struct FenwickTreePolynomial {
private:
private:
    int size, maxK;
    long long mod;
    vector<vector<long long>> BIT;
    vector<vector<long long>> cof {{0, 1},
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
    vector<vector<vector<long long>>> sumDiff;
    vector<long long> factorial;
    vector<vector<long long>> pascal;

    long long rsq(vector<long long> &array, int ind) {
        long long sum = 0;
        for (int x = ind; x > 0; x -= (x & -x)) sum = (sum + array[x]) % mod;
        return sum;
    }

    void update(vector<long long> &array, int ind, long long value) {
        value %= mod;
        for (int x = ind; x <= size; x += (x & -x)) array[x] = (array[x] + value) % mod;
    }

public:
    FenwickTreePolynomial(int size, int maxK, long long modbigPrime) : size(size), maxK(maxK), mod(modbigPrime), BIT(maxK + 2), sumDiff(maxK + 1), pascal(maxK + 2) {
        assert(maxK >= 0 && maxK <= 10);
        factorial.push_back(1);
        for (int i = 1; i < maxK + 2; i++) factorial.push_back(factorial.back() * i);
        for (int i = 0; i < maxK + 2; i++) for (int j = 0; j <= size; j++) BIT[i].push_back(0);
        for (int k = 0; k < maxK + 1; k++) {
            for (int i = 0; i < maxK + 2; i++) {
                sumDiff[k].push_back(vector<long long>());
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
    }

    /**
     * Range Sum query at index ind
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(K log(n))
     *
     * @param  ind index
     * @return sum
     */
    long long rsq(int ind) {
        long long sum = rsq(BIT[maxK + 1], ind);
        for (int i = maxK; i >= 0; i--) sum = ((sum * ind) % mod + rsq(BIT[i], ind)) % mod;
        if (sum < 0) sum += mod;
        sum = divMod(sum, factorial[maxK + 1], mod);
        return sum;
    }

    /**
     * Range Sum Query from a to b.
     * Search for the sum from array index from a to b
     * a and b are 1-indexed
     * <p>
     * Time-Complexity:    O(K log(n))
     *
     * @param  a left index
     * @param  b right index
     * @return sum
     */
    long long rsq(int a, int b) {
        long long sum = (rsq(b) - rsq(a - 1)) % mod;
        if (sum < 0) sum += mod;
        return sum;
    }

    /**
     * Update the array between index a and b with values v, 2^k * v, 3^k * v, ...
     * ind is 1-indexed
     * <p>
     * Time-Complexity:    O(K log(n))
     *
     * @param  a left index
     * @param  b right index
     * @param  value value
     * @param  k exponent
     */
    void update(int a, int b, long long value, int k) {
        value %= mod;
        long long s = a - 1, len = b - a + 1, mult;
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

    int getSize() {
        return size;
    }
};

#endif /* DATASTRUCTURES_TREES_FENWICKTREEPOLYNOMIAL_H_ */
