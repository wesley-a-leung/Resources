#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates and point queries
// Time Complexity:
//   init: O(PI(N_i))
//   update: O(2^D * PI(log(N_i)))
//   getValue: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// where PI is the product function, N_i is the size in the ith dimension, and D is the number of dimensions
template <class T, const bool ONE_INDEXED, const int ...Args> struct FenwickTreeRangePoint {
    T val;
    void init() { val = 0; }
    void update(T v) { val += v; }
    T getValue() { return val; }
};

template <class T, const bool ONE_INDEXED, const int MAXN, const int ...Ns> struct FenwickTreeRangePoint <T, ONE_INDEXED, MAXN, Ns...> {
    FenwickTreeRangePoint<T, ONE_INDEXED, Ns...> BIT[MAXN];
    void init() { for (int i = 0; i < MAXN; i++) BIT[i].init(); }
    template <class ...Args> void update(T v, int l, int r, Args ...args) {
        for (l += !ONE_INDEXED; l < MAXN; l += l & -l) BIT[l].update(v, args...);
        for (r += 1 + !ONE_INDEXED; r < MAXN; r += r & -r) BIT[r].update(-v, args...);
    }
    template <class ...Args> T getValue(int i, Args ...args) {
        T ret = 0;
        for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret += BIT[i].getValue(args...);
        return ret;
    }
};
