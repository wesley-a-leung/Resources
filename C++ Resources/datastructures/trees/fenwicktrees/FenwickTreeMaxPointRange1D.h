#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and range queries for maximum value queries
// Time Complexity:
//   init: O(N)
//   update, rmq(i): O(log N)
//   rmq(l, r): O((log N)^2)
// Memory Complexity: O(N)
template <const int MAXN, class T, const bool ONE_INDEXED> struct FenwickTreeMaxPointRange1D {
    T A[MAXN], BIT[MAXN], NEG_INF;
    T op(T a, T b) { return max(a, b); }
    FenwickTreeMaxPointRange1D(T NEG_INF) : NEG_INF(NEG_INF) { fill(A, A + MAXN, NEG_INF); fill(BIT, BIT + MAXN, NEG_INF); }
    void update(int i, T v) { for (i += !ONE_INDEXED, A[i] = op(A[i], v); i < MAXN; i += i & -i) BIT[i] = op(BIT[i], v); }
    T rmq(int i) { T ret = NEG_INF; for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret = op(ret, BIT[i]); return ret; }
    T rmq(int l, int r) {
        T ret = NEG_INF; l += !ONE_INDEXED; r += !ONE_INDEXED;
        while (l <= r) {
            if (l <= r - (r & -r) + 1) { ret = op(ret, BIT[r]); r -= r & -r; }
            else ret = op(ret, A[r--]);
        }
        return ret;
    }
};
