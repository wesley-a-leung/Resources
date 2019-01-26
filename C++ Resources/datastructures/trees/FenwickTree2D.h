#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and range queries in 2 dimensions
// Time Complexity:
//   init: O(NM)
//   update, rsq: O(log N log M)
// Memory Complexity: O(NM)
template <const int MAXN, const int MAXM, class T, const bool ONE_INDEXED> struct FenwickTree2D {
    T BIT[MAXN][MAXM];
    void init() { for (int i = 0; i < MAXN; i++) fill(BIT[i], BIT[i] + MAXM, 0); }
    void update(int x, int y, T v) {
        for (int i = x + !ONE_INDEXED; i < MAXN; i += i & -i) for (int j = y + !ONE_INDEXED; j < MAXM; j += j & -j) BIT[i][j] += v;
    }
    T rsq(int x, int y) {
        T ret = 0;
        for (int i = x + !ONE_INDEXED; i > 0; i -= i & -i) for (int j = y + !ONE_INDEXED; j > 0; j -= j & -j) ret += BIT[i][j];
        return ret;
    }
    T rsq(int x1, int y1, int x2, int y2) {
        return rsq(x2, y2) + rsq(x1 - 1, y1 - 1) - rsq(x1 - 1, y2) - rsq(x2, y1 - 1);
    }
};
