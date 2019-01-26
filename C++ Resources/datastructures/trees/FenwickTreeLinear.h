#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree supporting range updates with updates in the form of
// adding v, 2v, 3v, ... to the interval [l, r], and range sum queries
// Time Complexity:
//   constructor: O(N)
//   update, rsq: O(log N)
// Memory Complexity: O(N)
template <const int MAXN, class T, const bool ONE_INDEXED> struct FenwickTreeLinear {
    array<T, MAXN> con, lin, quad;
    T rsq(array<T, MAXN> &BIT, int i) { T ret = 0; for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret += BIT[i]; return ret; }
    void update(array<T, MAXN> &BIT, int i, T v) { for (i += !ONE_INDEXED; i < MAXN; i += i & -i) BIT[i] += v; }
    void init() { fill(con.begin(), con.end(), 0); fill(lin.begin(), lin.end(), 0); fill(quad.begin(), quad.end(), 0); }
    T rsq(int ind) { return (rsq(quad, ind) * (T) ind * (T) ind + rsq(lin, ind) * (T) ind + rsq(con, ind)) / (T) 2; }
    T rsq(int a, int b) { return rsq(b) - rsq(a - 1); }
    void update(int a, int b, T value) {
        int s = a - 1, len = b - a + 1;
        update(quad, a, value); update(quad, b + 1, -value);
        update(lin, a, value * ((T) 1 - (T) 2 * (T) s)); update(lin, b + 1, -value * ((T) 1 - (T) 2 * (T) s));
        update(con, a, value * ((T) s * (T) s - (T) s)); update(con, b + 1, -value * (((T) s * (T) s - (T) s)));
        update(con, b + 1, value * ((T) len * (T) (len + 1)));
    }
};
