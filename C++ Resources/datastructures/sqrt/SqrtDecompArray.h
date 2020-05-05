#pragma once
#include <bits/stdc++.h>
using namespace std;

// Sqrt Decomposes an array into blocks of size B to support point updates and range queries
// Time Complexity:
//   init: O(N)
//   update: O(B)
//   updateAssocLazy: O(1)
//   query: O(B + N / B)
// Memory Complexity: O(N + sqrt N)
template <const int MAXN, const int BSZ, const bool ONE_INDEXED> struct SqrtDecompArray {
    using Data = int; using Lazy = int; const Data qdef = 0;
    Data merge(const Data &l, const Data &r);
    Data applyLazy(const Data &l, const Lazy &r);
    int N; Data small[MAXN], large[MAXN / BSZ + 5];
    template <class It> void init(It st, It en) {
        N = en - st; fill(large, large + (N - 1) / BSZ + 1, qdef);
        for (int i = 0; i < N; i++) { small[i] = *(st + i); int bl = i / BSZ; large[bl] = merge(large[bl], small[i]); }
    }
    void init(int size, const Data &vdef) {
        N = size; fill(large, large + (N - 1) / BSZ + 1, qdef);
        for (int i = 0; i < N; i++) { small[i] = vdef; int bl = i / BSZ; large[bl] = merge(large[bl], small[i]); }
    }
    void update(int i, const Lazy &v) {
        i -= ONE_INDEXED; small[i] = applyLazy(small[i], v); int bl = i / BSZ; large[bl] = qdef;
        for (int j = bl * BSZ, en = min(j + BSZ, N); j < en; j++) large[bl] = merge(large[bl], small[j]);
    }
    void updateAssocLazy(int i, const Lazy &v) { // if lazy updates are asssociative
        i -= ONE_INDEXED; small[i] = applyLazy(small[i], v); int bl = i / BSZ; large[bl] = applyLazy(large[bl], v);
    }
    Data query(int l, int r) {
        Data ret = qdef; l -= ONE_INDEXED, r -= ONE_INDEXED;
        for (; l % BSZ && l <= r; l++) ret = merge(ret, small[l]);
        for (int bl = l / BSZ; l + BSZ - 1 <= r; l += BSZ, bl++) ret = merge(ret, large[bl]);
        for (; l <= r; l++) ret = merge(ret, small[l]);
        return ret;
    }
};

// Sqrt Decomposes an array into blocks of size B to support range updates and range queries
// Time Complexity:
//   init: O(N)
//   update: O(B + N / B)
//   query: O(B + N / B)
// Memory Complexity: O(N + sqrt N)
template <const int MAXN, const int BSZ, const bool ONE_INDEXED> struct SqrtDecompArrayLazy {
    using Data = int; using Lazy = int; const Data qdef = 0; const Lazy ldef = 0;
    Data merge(const Data &l, const Data &r); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    int N; Data small[MAXN], large[MAXN / BSZ + 5]; Lazy lz[MAXN / BSZ + 5];
    template <class It> void init(It st, It en) {
        N = en - st; fill(large, large + (N - 1) / BSZ + 1, qdef); fill(lz, lz + (N - 1) / BSZ + 1, ldef);
        for (int i = 0; i < N; i++) { small[i] = *(st + i); int bl = i / BSZ; large[bl] = merge(large[bl], small[i]); }
    }
    void init(int size, const Data &vdef) {
        N = size; fill(large, large + (N - 1) / BSZ + 1, qdef); fill(lz, lz + (N - 1) / BSZ + 1, ldef);
        for (int i = 0; i < N; i++) { small[i] = vdef; int bl = i / BSZ; large[bl] = merge(large[bl], small[i]); }
    }
    void propagate(int i) {
        int bl = i / BSZ;
        if (lz[bl] == ldef) return;
        for (int j = bl * BSZ, en = min(j + BSZ, N); j < en; j++) small[j] = applyLazy(small[j], lz[bl]);
        lz[bl] = ldef;
    }
    void pushup(int i) {
        int bl = i / BSZ;
        if (lz[bl] != ldef) return;
        large[bl] = qdef;
        for (int j = bl * BSZ, en = min(j + BSZ, N); j < en; j++) large[bl] = merge(large[bl], small[j]);
    }
    void update(int l, int r, const Lazy &v) {
        int l0 = l -= ONE_INDEXED, r0 = r -= ONE_INDEXED; propagate(l0); propagate(r0); Lazy blv = getSegmentVal(v, BSZ);
        for (; l % BSZ && l <= r; l++) small[l] = applyLazy(small[l], v);
        for (int bl = l / BSZ; l + BSZ - 1 <= r; l += BSZ, bl++) { large[bl] = applyLazy(large[bl], blv); lz[bl] = mergeLazy(lz[bl], v); }
        for (; l <= r; l++) small[l] = applyLazy(small[l], v);
        pushup(l0); pushup(r0);
    }
    Data query(int l, int r) {
        propagate(l -= ONE_INDEXED); propagate(r -= ONE_INDEXED); Data ret = qdef;
        for (; l % BSZ && l <= r; l++) ret = merge(ret, small[l]);
        for (int bl = l / BSZ; l + BSZ - 1 <= r; l += BSZ, bl++) ret = merge(ret, large[bl]);
        for (; l <= r; l++) ret = merge(ret, small[l]);
        return ret;
    }
};
