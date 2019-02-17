#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates and range queries in 3 dimensions
// Time Complexity:
//   init: O(NMP)
//   update, rsq: O((log N) (log M) (log P))
// Memory Complexity: O(NMP)
template <const int MAXN, const int MAXM, const int MAXP, class T, const bool ONE_INDEXED> struct FenwickTree3D {
    T BIT[MAXN][MAXM][MAXP]; void init() { for (int i = 0; i < MAXN; i++) for (int j = 0; j < MAXM; j++) fill(BIT[i][j], BIT[i][j] + MAXP, 0); }
    void update(int x, int y, int z, T v) {
        for (int i = x + !ONE_INDEXED; i < MAXN; i += i & -i) for (int j = y + !ONE_INDEXED; j < MAXM; j += j & -j)
            for (int k = z + !ONE_INDEXED; k < MAXP; k += k & -k) BIT[i][j][k] += v;
    }
    T rsq(int x, int y, int z) {
        T ret = 0;
        for (int i = x + !ONE_INDEXED; i > 0; i -= i & -i) for (int j = y + !ONE_INDEXED; j > 0; j -= j & -j)
            for (int k = z + !ONE_INDEXED; k > 0; k -= k & -k) ret += BIT[i][j][k];
        return ret;
    }
    T rsq(int x1, int y1, int z1, int x2, int y2, int z2) {
        return rsq(x2, y2, z2)
             - rsq(x1 - 1, y2, z2) - rsq(x2, y1 - 1, z2) - rsq(x2, y2, z1 - 1)
             + rsq(x1 - 1, y1 - 1, z2) + rsq(x1 - 1, y2, z1 - 1) + rsq(x2, y1 - 1, z1 - 1)
             - rsq(x1 - 1, y1 - 1, z1 - 1);
    }
};
