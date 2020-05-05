#pragma once
#include <bits/stdc++.h>
using namespace std;

// point updates, range query
template <const int MAXN, const bool ONE_INDEXED> struct SegmentTreeBottomUp {
    using Data = int; using Lazy = int; int N; Data T[MAXN << 1]; const Data qdef = 0;
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    template <class It> void init(It st, It en) {
        N = en - st; for (int i = 0; i < N; i++) T[N + i] = *(st + i);
        for (int i = N - 1; i > 0; i--) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }
    void init(int size, const Data &vdef) {
        N = size; for (int i = 0; i < N; i++) T[N + i] = vdef;
        for (int i = N - 1; i > 0; i--) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }
    void update(int i, const Lazy &v) { for (i += N - ONE_INDEXED, T[i] = applyLazy(T[i], v); i >>= 1;) T[i] = merge(T[i << 1], T[i << 1 | 1]); }
    Data query(int l, int r) {
        Data ql = qdef, qr = qdef;
        for (l += N - ONE_INDEXED, r += N - ONE_INDEXED; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ql = merge(ql, T[l++]);
            if (!(r & 1)) qr = merge(T[r--], qr);
        }
        return merge(ql, qr);
    }
};

// range updates, range query
template <const int MAXN, const bool ONE_INDEXED> struct SegmentTreeLazyBottomUp {
    using Data = int; using Lazy = int; int N, H; Data T[MAXN << 1]; Lazy L[MAXN]; const Data qdef = 0; const Lazy ldef = 0;
    Data merge(const Data &l, const Data &r); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    void apply(int i, const Lazy &v, int k) {
        T[i] = applyLazy(T[i], getSegmentVal(v, k));
        if (i < N) L[i] = mergeLazy(L[i], v);
    }
    void pushup(int i) {
        for (int k = 2; i >>= 1; k <<= 1) {
            T[i] = L[i] == ldef ? merge(T[i << 1], T[i << 1 | 1]) : applyLazy(merge(T[i << 1], T[i << 1 | 1]), getSegmentVal(L[i], k));
        }
    }
    void propagate(int i) {
        for (int h = H, k = 1 << (H - 1), ii = i >> h; h > 0; ii = i >> --h, k >>= 1) if (L[ii] != ldef) {
            apply(ii << 1, L[ii], k); apply(ii << 1 | 1, L[ii], k); L[ii] = ldef;
        }
    }
    template <class It> void init(It st, It en) {
        N = en - st; H = 32 - __builtin_clz(N);
        for (int i = 0; i < N; i++) { T[N + i] = *(st + i); L[i] = ldef; }
        for (int i = N - 1; i > 0; i--) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }
    void init(int size, const Data &vdef) {
        N = size; H = 32 - __builtin_clz(N);
        for (int i = 0; i < N; i++) { T[N + i] = vdef; L[i] = ldef; }
        for (int i = N - 1; i > 0; i--) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }
    void update(int l, int r, const Lazy &v) {
        int l0 = l += N - ONE_INDEXED, r0 = r += N - ONE_INDEXED, k = 1; propagate(l); propagate(r);
        for (; l <= r; l >>= 1, r >>= 1, k <<= 1) {
            if (l & 1) apply(l++, v, k);
            if (!(r & 1)) apply(r--, v, k);
        }
        pushup(l0); pushup(r0);
    }
    Data query(int l, int r) {
        propagate(l += N - ONE_INDEXED); propagate(r += N - ONE_INDEXED); Data ql = qdef, qr = qdef;
        for (; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ql = merge(ql, T[l++]);
            if (!(r & 1)) qr = merge(T[r--], qr);
        }
        return merge(ql, qr);
    }
};
