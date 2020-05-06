#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and range queries in 1 dimension
// indices are 0-indexed and ranges are inclusive
// Time Complexity:
//   constructor, values: O(N)
//   update, rsq, bsearch, lower_bound, upper_bound: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/1279/problem/B
//   https://codeforces.com/contest/961/problem/E
template <class T> struct FenwickTree1D {
    int N; vector<T> BIT; FenwickTree1D(int N) : N(N), BIT(N + 1, T()) {}
    template <class It> FenwickTree1D(It st, It en) : FenwickTree1D(en - st) {
        for (int i = 1; i <= N; i++) {
            BIT[i] += *(st + i - 1); int j = i + (i & -i);
            if (j <= N) BIT[j] += BIT[i];
        }
    }
    vector<T> values() {
        vector<T> ret(BIT.begin() + 1, BIT.end());
        for (int i = N; i >= 1; i--) {
            int j = i + (i & -i);
            if (j <= N) ret[j - 1] -= ret[i - 1];
        }
        return ret;
    }
    void update(int i, T v) { for (i++; i <= N; i += i & -i) BIT[i] += v; }
    T rsq(int i) { T ret = T(); for (i++; i > 0; i -= i & -i) ret += BIT[i]; return ret; }
    T rsq(int l, int r) { return rsq(r) - rsq(l - 1); }
    template <class F> int bsearch(T v, F cmp) { // returns first index where cmp returns false, N if no such index exists
        T sum = T(); int ind = 0;
        for (int j = __lg(N + 1); j >= 0; j--) {
            int i = ind + (1 << j);
            if (i <= N && cmp(sum + BIT[i], v)) sum += BIT[ind = i];
        }
        return ind;
    }
    int lower_bound(T v) { return bsearch(v, less<T>()); }
    int upper_bound(T v) { return bsearch(v, less_equal<T>()); }
};
