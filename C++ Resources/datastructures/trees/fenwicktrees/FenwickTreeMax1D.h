#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and prefix range queries over a cumulative function, such as max and min
// Time Complexity:
//   constructor: O(N)
//   update, rmq: O(log N)
// Memory Complexity: O(N)
// Tested On:
//   https://atcoder.jp/contests/dp/tasks/dp_q
template <class T, class F> struct FenwickTreeMax1D {
    int N; vector<T> BIT; F op;
    FenwickTreeMax1D(int N, T vdef, F op) : N(N), BIT(N + 1, vdef), op(op) {}
    void update(int i, T v) { for (i++; i <= N; i += i & -i) BIT[i] = op(BIT[i], v); }
    T rmq(int i) { T ret = BIT[++i]; while ((i -= i & -i) > 0) ret = op(ret, BIT[i]); return ret; }
};
