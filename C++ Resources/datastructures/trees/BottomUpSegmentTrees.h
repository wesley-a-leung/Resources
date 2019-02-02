#pragma once
#include <bits/stdc++.h>
using namespace std;

// single assignment and modifications, range query
template <const int MAXN, const bool ONE_INDEXED> struct SegmentTree_SAM_RQ {
    using Data = int; int N; Data T[2 * MAXN];
    const Data def = 0; // default value
    const Data qdef = INT_MIN; // query default value
    // operation must be associative (but not necessarily commutative)
    Data merge(Data l, Data r); // to be implemented
    Data applyVal(Data l, Data r); // to be implemented
    template <class It> void init(It st, It en) {
        N = en - st; for (int i = 0; i < N; i++) T[N + i] = *(st + i);
        for (int i = N - 1; i > 0; i--) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }
    void init(int size) { N = size; for (int i = 1; i < 2 * N; i++) T[i] = def; }
    void update(int i, Data v) { for (i += N - ONE_INDEXED, T[i] = applyVal(T[i], v); i >>= 1;) T[i] = merge(T[i << 1], T[i << 1 | 1]); }
    Data query(int l, int r) {
        Data ql = qdef, qr = qdef;
        for (l += N - ONE_INDEXED, r += N - ONE_INDEXED; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) ql = merge(ql, T[l++]);
            if (!(r & 1)) qr = merge(T[r--], qr);
        }
        return merge(ql, qr);
    }
};

// range modification, single query
// works when order of modifications does not affect result
template <const int MAXN, const bool ONE_INDEXED> struct SegmentTree_RM_SQ {
    using Data = int; int N; Data T[2 * MAXN];
    const Data def = 0; // default value
    bool final = false; // whether the pushAll function has been called
    Data merge(Data l, Data r); // to be implemented
    template <class It> void init(It st, It en) { N = en - st; for (int i = 0; i < N; i++) T[N + i] = *(st + i), T[i] = 0; }
    void init(int size) { N = size; for (int i = 0; i < N; i++) T[N + i] = def, T[i] = 0; }
    void update(int l, int r, Data v) {
        for (l += N - ONE_INDEXED, r += N - ONE_INDEXED; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) { T[l] = merge(T[l], v); l++; }
            if (!(r & 1)) { T[r] = merge(T[r], v); r--; }
        }
    }
    Data query(int i) {
        if (final) return T[N + i - ONE_INDEXED];
        Data q = 0; for (i += N - ONE_INDEXED; i > 0; i >>= 1) q = merge(q, T[i]);
        return q;
    }
    void pushAll() {
        for (int i = 1; i < N; i++) { T[i << 1] = merge(T[i << 1], T[i]); T[i << 1 | 1] = merge(T[i << 1 | 1], T[i]); T[i] = 0; }
        final = true;
    }
};

// range assignments/modifications, range query
template <const int MAXN, const bool ONE_INDEXED> struct SegmentTree_RAM_RQ {
    using Data = int; using Lazy = int; int N, H; Data T[2 * MAXN]; Lazy L[MAXN]; // L stores lazy values
    const Data def = 0; // default value
    const Data qdef = 0; // query default value
    const Lazy ldef = 0; // lazy default value
    // operation must be associative (but not necessarily commutative)
    Data merge(Data l, Data r); // to be implemented
    Lazy getSegmentVal(Lazy v, int k); // to be implemented
    Lazy mergeLazy(Lazy l, Lazy r); // to be implemented
    Data applyLazy(Data l, Lazy r); // to be implemented
    void apply(int i, Lazy v, int k) {
        T[i] = applyLazy(T[i], getSegmentVal(v, k));
        if (i < N) L[i] = mergeLazy(L[i], v);
    }
    void pushup(int i) {
        for (int k = 2; i > 1; k <<= 1) {
            i >>= 1;
            T[i] = L[i] == ldef ? merge(T[i << 1], T[i << 1 | 1]) : applyLazy(merge(T[i << 1], T[i << 1 | 1]), getSegmentVal(L[i], k));
        }
    }
    void propagate(int i) {
        for (int h = H, k = 1 << (H - 1); h > 0; h--, k >>= 1) {
            int ii = i >> h;
            if (L[ii] != ldef) { apply(ii << 1, L[ii], k); apply(ii << 1 | 1, L[ii], k); L[ii] = ldef; }
        }
    }
    template <class It> void init(It st, It en) {
        N = en - st; H = 0; for (int i = 1; i <= N; H++) i <<= 1;
        for (int i = 0; i < N; i++) { T[N + i] = *(st + i); L[i] = ldef; }
        for (int i = N - 1; i > 0; i--) T[i] = merge(T[i << 1], T[i << 1 | 1]);
    }
    void init(int size) {
        N = size; H = 0;
        for (int i = 1; i <= N; H++) i <<= 1;
        for (int i = 1; i < 2 * N; i++) T[i] = def;
        for (int i = 0; i < N; i++) L[i] = ldef;
    }
    void update(int l, int r, Lazy v) {
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

template <class T> struct MaxContiguousSubarrayData { T pre, suf, sum, maxSum; bool isNull; };
template <class T> MaxContiguousSubarrayData<T> nullNode() {
    MaxContiguousSubarrayData<T> ret; ret.pre = ret.suf = ret.sum = ret.maxSum = 0; ret.isNull = true; return ret;
}
template <class T> MaxContiguousSubarrayData<T> merge(const MaxContiguousSubarrayData<T> &l, const MaxContiguousSubarrayData<T> &r) {
    if (l.isNull) return r;
    if (r.isNull) return l;
    MaxContiguousSubarrayData<T> ret; ret.isNull = false; ret.pre = max(l.pre, l.sum + r.pre); ret.suf = max(l.suf + r.sum, r.suf);
    ret.sum = l.sum + r.sum; ret.maxSum = max(max(l.maxSum, r.maxSum), l.suf + r.pre); return ret;
}
// range assignments
template <class T> MaxContiguousSubarrayData<T> applyLazy(const MaxContiguousSubarrayData<T> &x, const T &v) {
    MaxContiguousSubarrayData<T> ret; ret.pre = ret.suf = ret.sum = ret.maxSum = v; ret.isNull = false; return ret;
}
template <class T> T getSegmentVal(const T &l, int len) { return l * len; }
template <class T> T mergeLazy(const T &l, const T &r) { return r; }
