#pragma once
#include <bits/stdc++.h>
#include "FenwickTreeRangePoint1D.h"
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates and range queries in 1 dimension
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor: O(N)
//   update, rsq: O(log N)
// Memory Complexity: O(N)
// Tested:
//   http://www.usaco.org/index.php?page=viewproblem2&cpid=973
template <class T> struct FenwickTreeRange1D {
    vector<FenwickTreeRangePoint1D<T>> FT; FenwickTreeRange1D(int N) : FT(2, FenwickTreeRangePoint1D<T>(N)) {}
    T rsq(int i) { return FT[1].get(i) * T(i) + FT[0].get(i); }
    T rsq(int l, int r) { return rsq(r) - rsq(l - 1); }
    void update(int l, int r, T v) { FT[1].update(l, r, v); FT[0].update(l, v * T(1 - l)); FT[0].update(r + 1, v * T(r)); }
};
