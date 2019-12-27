#pragma once
#include <bits/stdc++.h>
using namespace std;

// FenwickTree supporting point updates and range queries, as well as
// support for lower_bound and upper_bound binary searching the cumulative sum
// Time Complexity:
//   init: O(N)
//   update, rsq, lower_bound, upper_bound: O(log N)
// Memory Complexity: O(N)
template <const int MAXN, class T, const bool ONE_INDEXED> struct FenwickTreeBinarySearch {
    int N, lg; T BIT[MAXN];
    void init(int size) { N = size; lg = 31 - __builtin_clz(N + 1); fill(BIT, BIT + N + 1, 0); }
    void update(int i, T v) { for (i += !ONE_INDEXED; i <= N; i += i & -i) BIT[i] += v; }
    T rsq(int i) { T ret = 0; for (i += !ONE_INDEXED; i > 0; i -= i & -i) ret += BIT[i]; return ret; }
    T rsq(int a, int b) { return rsq(b) - rsq(a - 1); }
    // returns the first index with prefix sum greater than or equal to value
    // or N + 1 - !ONE_INDEXED if none exists 
    int lower_bound(T value) {
        int ind = 0, i = 0; T sum = 0;
        for (int x = lg; x >= 0; x--) {
            i = ind | (1 << x);
            if (i > N) continue;
            if (sum + BIT[i] < value) { ind = i; sum += BIT[i]; }
        }
        return ind + 1 - !ONE_INDEXED;
    }
    // returns the first index with prefix sum greater than value
    // or N + 1 - !ONE_INDEXED if none exists
    int upper_bound(T value) {
        int ind = 0, i = 0; T sum = 0;
        for (int x = lg; x >= 0; x--) {
            i = ind | (1 << x);
            if (i > N) continue;
            if (sum + BIT[i] <= value) { ind = i; sum += BIT[i]; }
        }
        return ind + 1 - !ONE_INDEXED;
    }
};
