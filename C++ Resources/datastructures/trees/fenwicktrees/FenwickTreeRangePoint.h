#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates and point queries in any number of dimensions
// Indices are 0-indexed and ranges are inclusive
// In practice, this version performs as well as the 1D version
// Very small constant, liked most fenwick trees
// Time Complexity:
//   constructor: O(PI(N_i))
//   update: O(2^D * PI(log(N_i)))
//   getValue: O(PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
// where PI is the product function, N_i is the size in the ith dimension, and D is the number of dimensions
// Tested:
//   https://mcpt.ca/problem/adifferenceproblem
template <const int D, class T> struct FenwickTreeRangePoint {
    int N; vector<FenwickTreeRangePoint<D - 1, T>> BIT;
    template <class... Args> FenwickTreeRangePoint(int N, Args... args) : N(N), BIT(N + 1, FenwickTreeRangePoint<D - 1, T>(args...)) {}
    template <class... Args> void update(T v, int l, int r, Args... args) {
        for (l++; l <= N; l += l & -l) BIT[l].update(v, args...);
        for (r += 2; r <= N; r += r & -r) BIT[r].update(-v, args...);
    }
    template <class... Args> T get(int i, Args... args) {
        T ret = T();
        for (i++; i > 0; i -= i & -i) ret += BIT[i].get(args...);
        return ret;
    }
};

template <class T> struct FenwickTreeRangePoint<0, T> {
    T val; FenwickTreeRangePoint() : val(T()) {}
    void update(T v) { val += v; }
    T get() { return val; }
};
