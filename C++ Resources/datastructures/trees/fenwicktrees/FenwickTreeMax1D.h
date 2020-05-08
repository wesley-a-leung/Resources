#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and prefix range queries over a cumulative function, such as max and min
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(N)
//   update, rmq: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://atcoder.jp/contests/dp/tasks/dp_q
template <class T, class F> struct FenwickTreeMax1D {
    int N; vector<T> BIT; F op;
    FenwickTreeMax1D(int N, T vdef, F op) : N(N), BIT(N + 1, vdef), op(op) {}
    void update(int i, T v) { for (i++; i <= N; i += i & -i) BIT[i] = op(BIT[i], v); }
    T rmq(int r) { T ret = BIT[++r]; while ((r -= r & -r) > 0) ret = op(ret, BIT[r]); return ret; }
};
