#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates and point queries in 1 dimension
// Time Complexity:
//   init: O(N)
//   update, getValue: O(log N)
// Memory Complexity: O(N)
template <const int MAXN, class T, const bool ONE_INDEXED> struct FenwickTreeRangePoint1D {
    T BIT[MAXN]; void init() { fill(BIT, BIT + MAXN, 0); }
    void update(int i, T v) { for (i += !ONE_INDEXED; i < MAXN; i += i & -i) BIT[i] += v; }
    void update(int a, int b, T v) { update(a, v), update(b + 1, -v); }
    T getValue(int i) { T ret = 0; for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret += BIT[i]; return ret; }
};
