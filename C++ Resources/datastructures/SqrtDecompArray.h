#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sqrt Decomposes an array into blocks of size B to answer queries
// Time Complexity:
//   init: O(N)
//   update: O(B)
//   updateAssocLazy: O(1)
//   query: O(B + N / B)
// Memory Complexity: O(N + sqrt N)
template <const int MAXN, const int BLOCKSZ, const bool ONE_INDEXED> struct SqrtDecompArray {
    using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0;
    Data merge(const Data &l, const Data &r);
    Data applyLazy(const Data &l, const Lazy &r);
    int N; Data small[MAXN], large[MAXN / BLOCKSZ + 5];
    template <class It> void init(It st, It en) {
        N = en - st; fill(large, large + (N - 1) / BLOCKSZ + 1, qdef);
        for (int i = 0; i < N; i++) { small[i] = *(st + i); int bl = i / BLOCKSZ; large[bl] = merge(large[bl], small[i]); }
    }
    void init(int size) {
        N = size; fill(large, large + (N - 1) / BLOCKSZ + 1, qdef);
        for (int i = 0; i < N; i++) { small[i] = vdef; int bl = i / BLOCKSZ; large[bl] = merge(large[bl], small[i]); }
    }
    void update(int i, const Lazy &v) {
        i -= ONE_INDEXED; small[i] = applyLazy(small[i], v); int bl = i / BLOCKSZ; large[bl] = qdef;
        for (int j = bl * BLOCKSZ, en = min(j + BLOCKSZ, N); j < en; j++) large[bl] = merge(large[bl], small[j]);
    }
    void updateAssocLazy(int i, const Lazy &v) { // if lazy updates are asssociative
        i -= ONE_INDEXED; small[i] = applyLazy(small[i], v); int bl = i / BLOCKSZ; large[bl] = applyLazy(large[bl], v);
    }
    Data query(int l, int r) {
        Data ret = qdef;
        for (l -= ONE_INDEXED, r -= ONE_INDEXED; l % BLOCKSZ && l <= r; l++) ret = merge(ret, small[l]);
        for (; l + BLOCKSZ - 1 <= r; l += BLOCKSZ) ret = merge(ret, large[l / BLOCKSZ]);
        for (; l <= r; l++) ret = merge(ret, small[l]);
        return ret;
    }
};
