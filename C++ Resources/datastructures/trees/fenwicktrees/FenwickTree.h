#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and range queries in any number of dimensions
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(PI(N_i))
//   update: O(PI(log(N_i)))
//   rsq: O(2^D * PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// where PI is the product function, N_i is the size in the ith dimension, and D is the number of dimensions
// Tested:
//   https://dmoj.ca/problem/ioi01p1
//   https://dmoj.ca/problem/gfssoc1s4
//   https://dmoj.ca/problem/fallingsnowflakes
template <class T, const int D> struct FenwickTree {
    int N; vector<FenwickTree<T, D - 1>> BIT;
    template <class... Args> FenwickTree(int N, Args... args) : N(N), BIT(N + 1, FenwickTree<T, D - 1>(args...)) {}
    template <class... Args> void update(int i, Args... args) { for (i++; i <= N; i += i & -i) BIT[i].update(args...); }
    template <class... Args> T rsq(int l, int r, Args... args) {
        T ret = T();
        for (; l > 0; l -= l & -l) ret -= BIT[l].rsq(args...);
        for (r++; r > 0; r -= r & -r) ret += BIT[r].rsq(args...);
        return ret;
    }
};

template <class T> struct FenwickTree<T, 0> {
    T val; FenwickTree() : val(T()) {}
    void update(T v) { val += v; }
    T rsq() { return val; }
};
