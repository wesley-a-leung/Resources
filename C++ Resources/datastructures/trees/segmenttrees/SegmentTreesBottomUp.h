#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bottom up segment tree supporting point updates and range queries
// Indices are 0-indexed and ranges are inclusive
// In practice, has a small constant, not quite as fast as fenwick trees, but faster than top down segment trees
// A combine struct is provided with typedefs/using for data and lazy, a query default value (qdef),
// and implementations of merge and applyLazy
// Below is a sample struct for point increments and range max queries
// struct Combine {
//     using Data = int;
//     using Lazy = int;
//     const Data qdef = numeric_limits<int>::min();
//     Data merge(const Data &l, const Data &r) const { return max(l, r); }
//     Data applyLazy(const Data &l, const Lazy &r) const { return l + r; }
// };
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds3
//   https://dmoj.ca/problem/dmpg17g2
//   https://dmoj.ca/problem/coci17c1p5
//   https://dmoj.ca/problem/cco20p5
//   https://www.spoj.com/problems/BRCKTS/
template <class Combine> struct SegmentTreeBottomUp {
    using Data = typename Combine::Data; using Lazy = typename Combine::Lazy; Combine C; int N; vector<Data> TR;
    template <class It> SegmentTreeBottomUp(It st, It en) : N(en - st), TR(N << 1, C.qdef) {
        copy(st, en, TR.begin() + N);
        for (int i = N - 1; i > 0; i--) TR[i] = C.merge(TR[i << 1], TR[i << 1 | 1]);
    }
    SegmentTreeBottomUp(int N, const Data &vdef) : N(N), TR(N << 1, C.qdef) {
        fill(TR.begin() + N, TR.end(), vdef);
        for (int i = N - 1; i > 0; i--) TR[i] = C.merge(TR[i << 1], TR[i << 1 | 1]);
    }
    void update(int i, const Lazy &v) { for (i += N, TR[i] = C.applyLazy(TR[i], v); i >>= 1;) TR[i] = C.merge(TR[i << 1], TR[i << 1 | 1]); }
    Data query(int l, int r) {
        Data ql = C.qdef, qr = C.qdef;
        for (l += N, r += N; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ql = C.merge(ql, TR[l++]);
            if (!(r & 1)) qr = C.merge(TR[r--], qr);
        }
        return C.merge(ql, qr);
    }
};

// Bottom up segment tree supporting range updates and range queries
// Indices are 0-indexed and ranges are inclusive
// In practice, has a small constant, not quite as fast as fenwick trees, and similar performance as top down segment trees
// A combine struct is provided with typedefs/using for data and lazy, a query default value (qdef), lazy default value (ldef),
// and implementations of merge, applyLazy, getSegmentVal, and mergeLazy
// Below is a sample struct for range assignment and range sum queries
// struct Combine {
//     using Data = int;
//     using Lazy = int;
//     const Data qdef = 0;
//     const Lazy ldef = numeric_limits<int>::min();
//     Data merge(const Data &l, const Data &r) const { return l + r; }
//     Data applyLazy(const Data &l, const Lazy &r) const { return r; }
//     Lazy getSegmentVal(const Lazy &v, int k) const { return v * k; }
//     Lazy mergeLazy(const Lazy &l, const Lazy &r) const { return r; }
// };
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/dmopc17c4p6
//   https://dmoj.ca/problem/dmopc18c5p5
//   https://dmoj.ca/problem/dmopc18c6p5
//   https://dmoj.ca/problem/lazy
//   https://mcpt.ca/problem/seq3
template <class Combine> struct SegmentTreeLazyBottomUp {
    using Data = typename Combine::Data; using Lazy = typename Combine::Lazy; Combine C; int N, lgN; vector<Data> TR; vector<Lazy> LZ;
    void apply(int i, const Lazy &v, int k) {
        TR[i] = C.applyLazy(TR[i], C.getSegmentVal(v, k));
        if (i < N) LZ[i] = C.mergeLazy(LZ[i], v);
    }
    void pushup(int i) {
        for (int k = 2; i >>= 1; k <<= 1) {
            TR[i] = C.merge(TR[i << 1], TR[i << 1 | 1]);
            if (LZ[i] != C.ldef) TR[i] = C.applyLazy(TR[i], C.getSegmentVal(LZ[i], k));
        }
    }
    void propagate(int i) {
        for (int h = lgN + 1, k = 1 << lgN, ii = i >> h; h > 0; ii = i >> --h, k >>= 1) if (LZ[ii] != C.ldef) {
            apply(ii << 1, LZ[ii], k); apply(ii << 1 | 1, LZ[ii], k); LZ[ii] = C.ldef;
        }
    }
    template <class It> SegmentTreeLazyBottomUp(It st, It en) : N(en - st), lgN(__lg(N)), TR(N << 1, C.qdef), LZ(N, C.ldef) {
        copy(st, en, TR.begin() + N);
        for (int i = N - 1; i > 0; i--) TR[i] = C.merge(TR[i << 1], TR[i << 1 | 1]);
    }
    SegmentTreeLazyBottomUp(int N, const Data &vdef) : N(N), lgN(__lg(N)), TR(N << 1, C.qdef), LZ(N, C.ldef) {
        fill(TR.begin() + N, TR.end(), vdef);
        for (int i = N - 1; i > 0; i--) TR[i] = C.merge(TR[i << 1], TR[i << 1 | 1]);
    }
    void update(int l, int r, const Lazy &v) {
        int l0 = l += N, r0 = r += N, k = 1; propagate(l); propagate(r);
        for (; l <= r; l >>= 1, r >>= 1, k <<= 1) {
            if (l & 1) apply(l++, v, k);
            if (!(r & 1)) apply(r--, v, k);
        }
        pushup(l0); pushup(r0);
    }
    Data query(int l, int r) {
        propagate(l += N); propagate(r += N); Data ql = C.qdef, qr = C.qdef;
        for (; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ql = C.merge(ql, TR[l++]);
            if (!(r & 1)) qr = C.merge(TR[r--], qr);
        }
        return C.merge(ql, qr);
    }
};
