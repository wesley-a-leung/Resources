#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and prefix range queries over a cumulative functor, such as max and min
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(PI(N_i))
//   update, rmq: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// where PI is the product function, N_i is the size in the ith dimension
// Tested:
//   https://dmoj.ca/problem/ccc19s5
template <class T, class C, const int D> struct FenwickTreeMax {
    int N; vector<FenwickTreeMax<T, C, D - 1>> BIT;
    template <class... Args> FenwickTreeMax(T vdef, int N, Args... args) : N(N), BIT(N + 1, FenwickTreeMax<T, C, D - 1>(vdef, args...)) {}
    template <class... Args> void update(int i, Args... args) { for (i++; i <= N; i += i & -i) BIT[i].update(args...); }
    template <class... Args> T rmq(int r, Args... args) {
        T ret = BIT[++r].rmq(args...);
        while ((r -= r & -r) > 0) ret = C()(ret, BIT[r].rmq(args...));
        return ret;
    }
};

template <class T, class C> struct FenwickTreeMax<T, C, 0> {
    T val; FenwickTreeMax(T vdef) : val(vdef) {}
    void update(T v) { val = C()(val, v); }
    T rmq() { return val; }
};
