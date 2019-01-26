#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and range queries
// Time Complexity:
//   init: O(N)
//   update, rsq: O(log N)
// Memory Complexity: O(N)
template <const int MAXN, class T, const bool ONE_INDEXED> struct FenwickTree {
    T BIT[MAXN];
    void init() { fill(BIT, BIT + MAXN, 0); }
    void update(int i, T v) { for (i += !ONE_INDEXED; i < MAXN; i += i & -i) BIT[i] += v; }
    T rsq(int i) { T ret = 0; for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret += BIT[i]; return ret; }
    T rsq(int a, int b) { return rsq(b) - rsq(a - 1); }
};
