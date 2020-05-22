#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and prefix range queries over a cumulative function,
// such as max and min, in 1 dimension
// Indices are 0-indexed and ranges are inclusive
// In practice, this version performs as well as the multidimensional version
// Very small constant, like most fenwick trees
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://atcoder.jp/contests/dp/tasks/dp_q
template <class T, class F> struct FenwickTreeCumulative1D {
    int N; vector<T> BIT; F op;
    FenwickTreeCumulative1D(int N, T vdef, F op) : N(N), BIT(N + 1, vdef), op(op) {}
    void update(int i, T v) { for (i++; i <= N; i += i & -i) BIT[i] = op(BIT[i], v); }
    T query(int r) { T ret = BIT[++r]; while ((r -= r & -r) > 0) ret = op(ret, BIT[r]); return ret; }
};
