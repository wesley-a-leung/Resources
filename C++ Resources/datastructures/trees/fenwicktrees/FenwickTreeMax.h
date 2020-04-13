#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and prefix range queries for maximum value queries in any number of dimensions
// Time Complexity:
//   init: O(PI(N_i))
//   update, rmq: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// where PI is the product function, N_i is the size in the ith dimension
using T = int; const T NEG_INF = 0;

T op(T a, T b) { return max(a, b); }

template <const bool ONE_INDEXED, const int ...Args> struct FenwickTreeMax {
    T val;
    void init() { val = NEG_INF; }
    void update(T v) { val = op(val, v); }
    T rmq() { return val; }
};

template <const bool ONE_INDEXED, const int MAXN, const int ...Ns> struct FenwickTreeMax <ONE_INDEXED, MAXN, Ns...> {
    FenwickTreeMax<ONE_INDEXED, Ns...> BIT[MAXN];
    void init() { for (int i = 0; i < MAXN; i++) BIT[i].init(); }
    template <class ...Args> void update(int i, Args ...args) { for (i += !ONE_INDEXED; i < MAXN; i += i & -i) BIT[i].update(args...); }
    template <class ...Args> T rmq(int i, Args ...args) {
        T ret = NEG_INF; for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret = op(ret, BIT[i].rmq(args...)); return ret;
    }
};
