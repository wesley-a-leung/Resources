#pragma once
#include <bits/stdc++.h>
using namespace std;

// Top down segment tree supporting point updates and range queries, with a lazy template argument to enable range updates
// Indices are 0-indexed and ranges are inclusive
// In practice, has a small constant, not quite as fast as fenwick trees or bottom up non lazy segment trees,
// and similar performance as bottom up lazy segment trees
// A combine struct is provided with typedefs/using for data and lazy, a query default value (qdef), lazy default value (ldef),
// and implementations of merge, applyLazy, getSegmentVal, and mergeLazy
// If LAZY is false, then only qdef, merge, and applyLazy are required to be implemented
// Below is a sample struct for point assignment and range sum queries
// struct Combine {
//     using Data = int;
//     using Lazy = int;
//     const Data qdef = 0;
//     Data merge(const Data &l, const Data &r) const { return l + r; }
//     Data applyLazy(const Data &l, const Lazy &r) const { return r; }
// };
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
//   https://dmoj.ca/problem/ds3 (LAZY = false)
//   https://dmoj.ca/problem/coci17c1p5 (LAZY = false)
//   https://dmoj.ca/problem/cco20p5 (LAZY = false)
//   https://www.spoj.com/problems/BRCKTS/ (LAZY = false)
//   https://dmoj.ca/problem/dmopc17c4p6 (LAZY = true)
//   https://dmoj.ca/problem/dmopc18c5p5 (LAZY = true)
//   https://dmoj.ca/problem/dmopc18c6p5 (LAZY = true)
//   https://dmoj.ca/problem/lazy (LAZY = true)
template <const bool LAZY, class Combine> struct SegmentTreeTopDown {
    using Data = typename Combine::Data; using Lazy = typename Combine::Lazy; Combine C; int N; vector<Data> TR; vector<Lazy> LZ;
    #define lazy_def template <const bool _ = LAZY> typename enable_if<_>::type
    #define not_lazy_def template <const bool _ = LAZY> typename enable_if<!_>::type
    lazy_def propagate(int x, int tl, int tr) {
        if (LZ[x] != C.ldef) {
            int m = tl + (tr - tl) / 2, rc = x + (m - tl + 1) * 2;
            TR[x + 1] = C.applyLazy(TR[x + 1], C.getSegmentVal(LZ[x], m - tl + 1)); LZ[x + 1] = C.mergeLazy(LZ[x + 1], LZ[x]);
            TR[rc] = C.applyLazy(TR[rc], C.getSegmentVal(LZ[x], tr - m)); LZ[rc] = C.mergeLazy(LZ[rc], LZ[x]);
            LZ[x] = C.ldef;
        }
    }
    not_lazy_def propagate(int, int, int) {}
    lazy_def apply(int x, int tl, int tr, const Lazy &v) {
        TR[x] = C.applyLazy(TR[x], C.getSegmentVal(v, tr - tl + 1)); LZ[x] = C.mergeLazy(LZ[x], v);
    }
    not_lazy_def apply(int x, int, int, const Lazy &v) { TR[x] = C.applyLazy(TR[x], v); }
    template <class F> void build(int x, int tl, int tr, F a) {
        if (tl == tr) { TR[x] = a(tl); return; }
        int m = tl + (tr - tl) / 2, rc = x + (m - tl + 1) * 2;
        build(x + 1, tl, m, a); build(rc, m + 1, tr, a); TR[x] = C.merge(TR[x + 1], TR[rc]);
    }
    void update(int x, int tl, int tr, int l, int r, const Lazy &v) {
        if (l <= tl && tr <= r) { apply(x, tl, tr, v); return; }
        propagate(x, tl, tr); int m = tl + (tr - tl) / 2, rc = x + (m - tl + 1) * 2;
        if (tl <= r && l <= m) update(x + 1, tl, m, l, r, v);
        if (m + 1 <= r && l <= tr) update(rc, m + 1, tr, l, r, v);
        TR[x] = C.merge(TR[x + 1], TR[rc]);
    }
    Data query(int x, int tl, int tr, int l, int r) {
        if (r < tl || tr < l) return C.qdef;
        if (l <= tl && tr <= r) return TR[x];
        propagate(x, tl, tr); int m = tl + (tr - tl) / 2, rc = x + (m - tl + 1) * 2;
        return C.merge(query(x + 1, tl, m, l, r), query(rc, m + 1, tr, l, r));
    }
    lazy_def initLazy() { LZ.assign(N * 2 - 1, C.ldef); }
    not_lazy_def initLazy() {}
    template <class It> SegmentTreeTopDown(It st, It en) : N(en - st), TR(N * 2 - 1, C.qdef) {
        initLazy(); build(0, 0, N - 1, [&] (int i) { return *(st + i); });
    }
    SegmentTreeTopDown(int N, const Data &vdef) : N(N), TR(N * 2 - 1, C.qdef) {
        initLazy(); build(0, 0, N - 1, [&] (int i) { return vdef; });
    }
    lazy_def update(int l, int r, const Lazy &v) { update(0, 0, N - 1, l, r, v); }
    void update(int i, const Lazy &v) { update(0, 0, N - 1, i, i, v); }
    Data query(int l, int r) { return query(0, 0, N - 1, l, r); }
};

// Top down dynamic segment tree supporting range updates and range queries
// Indices are 0-indexed and ranges are inclusive
// In practice, has a large constant
// A combine struct is provided with typedefs/using for data and lazy, a query default value (qdef), lazy default value (ldef),
// and implementations of merge, applyLazy, getSegmentVal, mergeLazy, and getSegmentVdef
// If LAZY is false, then only qdef, merge, applyLazy, and getSegmentVdef are required to be implemented
// Below is a sample struct for point assignment and range sum queries, where the default value of each index is 1
// struct Combine {
//     using Data = int;
//     using Lazy = int;
//     const Data qdef = 0;
//     Data merge(const Data &l, const Data &r) const { return l + r; }
//     Data applyLazy(const Data &l, const Lazy &r) const { return r; }
//     template <class IndexType> Data getSegmentVdef(IndexType k) const { return k; }
// };
// Below is a sample struct for range assignment and range sum queries, where the default value of each index is 1
// struct Combine {
//     using Data = int;
//     using Lazy = int;
//     const Data qdef = 0;
//     const Lazy ldef = numeric_limits<int>::min();
//     Data merge(const Data &l, const Data &r) const { return l + r; }
//     Data applyLazy(const Data &l, const Lazy &r) const { return r; }
//     template <class IndexType> Lazy getSegmentVal(const Lazy &v, IndexType k) const { return v * k; }
//     Lazy mergeLazy(const Lazy &l, const Lazy &r) const { return r; }
//     template <class IndexType> Data getSegmentVdef(IndexType k) const { return k; }
// };
// Time Complexity:
//   constructor: O(1) for single argument constructor, O(N) for two argument constructor
//   update: O(log N) amortized unless reserveNodes is called beforehand
//   query: O(log N)
// Memory Complexity: O(Q log N) for Q updates if single argument constructor is called, O(N + Q log N) for two argument constructor 
// Tested:
//   https://dmoj.ca/problem/ccc05s5 (LAZY = false, PERSISTENT = false)
//   https://wcipeg.com/problem/wc184s4 (LAZY = false, PERSISTENT = true)
//   https://codeforces.com/contest/1080/problem/F (LAZY = false, PERSISTENT = true)
//   https://codeforces.com/contest/915/problem/E (LAZY = true, PERSISTENT = false)
//   https://codeforces.com/gym/101982/problem/F (LAZY = true, PERSISTENT = false)
//   https://www.spoj.com/problems/TTM/ (LAZY = true, PERSISTENT = true)
template <class IndexType, const bool LAZY, const bool PERSISTENT, class Combine> struct SegmentTreeDynamic {
    using Data = typename Combine::Data; using Lazy = typename Combine::Lazy; Combine C; IndexType N;
    vector<int> L, R, roots; vector<Data> TR; vector<Lazy> LZ;
    #define lazy_def template <const bool _ = LAZY> typename enable_if<_>::type
    #define not_lazy_def template <const bool _ = LAZY> typename enable_if<!_>::type
    lazy_def pushBackLazy(int cp) { LZ.push_back(~cp ? LZ[cp] : C.ldef); }
    not_lazy_def pushBackLazy(int) {}
    int makeNode(int cp, IndexType tl, IndexType tr) {
        if (~cp) { L.push_back(L[cp]); R.push_back(R[cp]); TR.push_back(TR[cp]); }
        else { L.push_back(-1); R.push_back(-1); TR.push_back(C.getSegmentVdef(tr - tl + 1)); }
        pushBackLazy(cp); return int(TR.size()) - 1;
    }
    lazy_def propagate(int x, IndexType tl, IndexType tr) {
        if (LZ[x] != C.ldef) {
            IndexType m = tl + (tr - tl) / 2;
            if (!~L[x] || PERSISTENT) { int nl = makeNode(L[x], tl, m); L[x] = nl; }
            TR[L[x]] = C.applyLazy(TR[L[x]], C.getSegmentVal(LZ[x], m - tl + 1)); LZ[L[x]] = C.mergeLazy(LZ[L[x]], LZ[x]);
            if (!~R[x] || PERSISTENT) { int nr = makeNode(R[x], m + 1, tr); R[x] = nr; }
            TR[R[x]] = C.applyLazy(TR[R[x]], C.getSegmentVal(LZ[x], tr - m)); LZ[R[x]] = C.mergeLazy(LZ[R[x]], LZ[x]);
            LZ[x] = C.ldef;
        }
    }
    not_lazy_def propagate(int, IndexType, IndexType) {}
    lazy_def apply(int x, IndexType tl, IndexType tr, const Lazy &v) {
        TR[x] = C.applyLazy(TR[x], C.getSegmentVal(v, tr - tl + 1)); LZ[x] = C.mergeLazy(LZ[x], v);
    }
    not_lazy_def apply(int x, IndexType, IndexType, const Lazy &v) { TR[x] = C.applyLazy(TR[x], v); }
    template <class It> int build(IndexType tl, IndexType tr, It st) {
        int x = makeNode(-1, tl, tr);
        if (tl == tr) { TR[x] = *(st + tl); return x; }
        IndexType m = tl + (tr - tl) / 2; int nl = build(tl, m, st); L[x] = nl; int nr = build(m + 1, tr, st); R[x] = nr;
        TR[x] = C.merge(TR[L[x]], TR[R[x]]); return x;
    }
    int update(int y, IndexType tl, IndexType tr, IndexType l, IndexType r, const Lazy &v, int trSz) {
        int x = !~y || (PERSISTENT && y < trSz) ? makeNode(y, tl, tr) : y;
        if (l <= tl && tr <= r) { apply(x, tl, tr, v); return x; }
        propagate(x, tl, tr); IndexType m = tl + (tr - tl) / 2;
        if (tl <= r && l <= m) { int nl = update(L[x], tl, m, l, r, v, trSz); L[x] = nl; }
        if (m + 1 <= r && l <= tr) { int nr = update(R[x], m + 1, tr, l, r, v, trSz); R[x] = nr; }
        TR[x] = C.merge(~L[x] ? TR[L[x]] : C.getSegmentVdef(m - tl + 1), ~R[x] ? TR[R[x]] : C.getSegmentVdef(tr - m)); return x;
    }
    Data query(int x, IndexType tl, IndexType tr, IndexType l, IndexType r) {
        if (r < tl || tr < l) return C.qdef;
        if (!~x) return C.getSegmentVdef(tr - tl + 1);
        if (l <= tl && tr <= r) return TR[x];
        propagate(x, tl, tr); IndexType m = tl + (tr - tl) / 2; return C.merge(query(L[x], tl, m, l, r), query(R[x], m + 1, tr, l, r));
    }
    template <class It> SegmentTreeDynamic(It st, It en) : N(en - st) { reserveNodes(N * 2 - 1); roots.push_back(build(0, N - 1, st)); }
    SegmentTreeDynamic(IndexType N) : N(N) { roots.push_back(-1); }
    lazy_def update(IndexType l, IndexType r, const Lazy &v, bool newRoot) {
        int nr = update(roots.back(), 0, N - 1, l, r, v, TR.size());
        if (newRoot) roots.push_back(nr);
        else roots.back() = nr;
    }
    void update(IndexType i, const Lazy &v, bool newRoot) {
        int nr = update(roots.back(), 0, N - 1, i, i, v, TR.size());
        if (newRoot) roots.push_back(nr);
        else roots.back() = nr;
    }
    Data query(IndexType l, IndexType r, int rootInd = -1) { return query(~rootInd ? roots[rootInd] : roots.back(), 0, N - 1, l, r); }
    void revert(int rootInd) { roots.push_back(roots[rootInd]); }
    void reserveNodes(int k) { L.reserve(k); R.reserve(k); TR.reserve(k); if (LAZY) LZ.reserve(k); }
};
