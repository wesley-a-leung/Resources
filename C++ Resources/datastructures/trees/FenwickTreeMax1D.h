#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and prefix range queries for maximum value queries
// Time Complexity:
//   init: O(N)
//   update, rmq: O(log N)
// Memory Complexity: O(N)
template <const int MAXN, class T, const bool ONE_INDEXED> struct FenwickTreeMax1D {
    T BIT[MAXN], NEG_INF;
    T op(T a, T b) { return max(a, b); }
    FenwickTreeMax1D(T NEG_INF) : NEG_INF(NEG_INF) { fill(BIT, BIT + MAXN, NEG_INF); }
    void update(int i, T v) { for (i += !ONE_INDEXED; i < MAXN; i += i & -i) BIT[i] = op(BIT[i], v); }
    T rmq(int i) { T ret = NEG_INF; for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret = op(ret, BIT[i]); return ret; }
};
