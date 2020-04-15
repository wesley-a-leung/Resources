#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates and range queries in 1 dimension
// Time Complexity:
//   init: O(N)
//   update, rsq: O(log N)
// Memory Complexity: O(N)
template <const int MAXN, class T, const bool ONE_INDEXED> struct FenwickTreeRange1D {
    array<T, MAXN> BIT1, BIT2; void init() { BIT1.fill(0); BIT2.fill(0); }
    T rsq(array<T, MAXN> &BIT, int i) { T ret = 0; for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret += BIT[i]; return ret; }
    void update(array<T, MAXN> &BIT, int i, T v) { for (i += !ONE_INDEXED; i < MAXN; i += i & -i) BIT[i] += v; }
    T rsq(int ind) { return rsq(BIT1, ind) * ind - rsq(BIT2, ind); }
    T rsq(int a, int b) { return rsq(b) - rsq(a - 1); }
    void update(int a, int b, T v) {
        update(BIT1, a, v); update(BIT1, b + 1, -v);
        update(BIT2, a, v * T(a - 1)); update(BIT2, b + 1, -v * T(b));
    }
};