#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and range queries in any number of dimensions
// Time Complexity:
//   init: O(PI(N_i))
//   update, rsq: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// where PI is the product function, and N_i is the size in the ith dimension
template <class T, const bool ONE_INDEXED, const int... Args> struct FenwickTree {
    T val;
    void init() { val = 0; }
    void update(int v) { val += v; }
    T rsq() { return val; }
};

template <class T, const bool ONE_INDEXED, const int MAXN, const int... Ns> struct FenwickTree <T, ONE_INDEXED, MAXN, Ns...> {
    FenwickTree<T, ONE_INDEXED, Ns...> BIT[MAXN];
    void init() { for (int i = 0; i < MAXN; i++) BIT[i].init(); }
    template <typename... Args> void update(int i, Args... args) { for (i += !ONE_INDEXED; i < MAXN; i += i & -i) BIT[i].update(args...); }
    template <typename... Args> T rsq(int l, int r, Args... args) {
        T ret = 0;
        for (r += !ONE_INDEXED; r > 0; r -= r & -r) ret += BIT[r].rsq(args...);
        for (l -= ONE_INDEXED; l > 0; l -= l & -l) ret -= BIT[l].rsq(args...);
        return ret;
    }
};
