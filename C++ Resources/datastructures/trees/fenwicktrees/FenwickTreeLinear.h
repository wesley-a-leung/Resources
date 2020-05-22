#pragma once
#include <bits/stdc++.h>
#include "FenwickTreeRangePoint1D.h"
using namespace std;

// Fenwick Tree supporting range updates with updates in the form of
// adding m + b, 2m + b, 3m + b, ... to the interval [l, r], and range sum queries
// Indices are 0-indexed and ranges are inclusive
// Very small constant, like most fenwick trees
// Time Complexity:
//   constructor: O(N)
//   update, rsq: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/acc3p4
template <class T> struct FenwickTreeLinear {
    vector<FenwickTreeRangePoint1D<T>> FT; FenwickTreeLinear(int N) : FT(3, FenwickTreeRangePoint1D<T>(N)) {}
    T rsq(int r) { return (FT[2].get(r) * T(r) * T(r) + FT[1].get(r) * T(r) + FT[0].get(r)) / 2; }
    T rsq(int l, int r) { return rsq(r) - rsq(l - 1); }
    void update(int l, int r, T m, T b) {
        FT[2].update(l, r, m); FT[1].update(l, r, m * (T(1) - T(l - 1) * T(2)) + b * T(2));
        FT[0].update(l, m * T(l - 1) * T(l - 2) + b * T(1 - l) * T(2));
        FT[0].update(r + 1, m * (T(r - l + 1) * T(r - l + 2) - T(l - 1) * T(l - 2)) + b * T(r) * T(2));
    }
};
