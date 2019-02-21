#pragma once
#include <bits/stdc++.h>
using namespace std;

template <const int MAXN, const bool ONE_INDEXED> struct SegmentTree {
    using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0; Data T[MAXN * 2], A[MAXN]; int N;
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    void build(int cur, int tl, int cR) {
        if (tl == cR) { T[cur] = A[tl]; return; }
        int m = tl + (cR - tl) / 2, rc = cur + (m - tl + 1) * 2; build(cur + 1, tl, m); build(rc, m + 1, cR); T[cur] = merge(T[cur + 1], T[rc]);
    }
    void update(int cur, int tl, int cR, int ind, const Lazy &val) {
        if (tl > ind || cR < ind) return;
        if (tl == cR) { T[cur] = applyLazy(T[cur], val); return; }
        int m = tl + (cR - tl) / 2, rc = cur + (m - tl + 1) * 2; update(cur + 1, tl, m, ind, val); update(rc, m + 1, cR, ind, val);
        T[cur] = merge(T[cur + 1], T[rc]);
    }
    Data query(int cur, int tl, int cR, int l, int r) {
        if (tl > r || cR < l) return qdef;
        if (tl >= l && cR <= r) return T[cur];
        int m = tl + (cR - tl) / 2, rc = cur + (m - tl + 1) * 2; return merge(query(cur + 1, tl, m, l, r), query(rc, m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 0; i < N; i++) A[i + ONE_INDEXED] = *(st + i);
        build(0, ONE_INDEXED, N - !ONE_INDEXED);
    }
    void init(int size) { N = size; fill(A + ONE_INDEXED, A + N + ONE_INDEXED, vdef); build(0, ONE_INDEXED, N - !ONE_INDEXED); }
    void update(int ind, const Lazy &val) { update(0, ONE_INDEXED, N - !ONE_INDEXED, ind, val); }
    Data query(int l, int r) { return query(0, ONE_INDEXED, N - !ONE_INDEXED, l, r); }
};

template <const int MAXN, const bool ONE_INDEXED> struct LazySegmentTree {
    using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0; const Lazy ldef = 0; Data T[MAXN * 2], A[MAXN]; Lazy L[MAXN * 2]; int N;
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int len); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    void propagate(int cur, int tl, int cR) {
        if (L[cur] != ldef) {
            int m = tl + (cR - tl) / 2, rc = cur + (m - tl + 1) * 2;
            T[cur + 1] = applyLazy(T[cur + 1], getSegmentVal(L[cur], m - tl + 1)); L[cur + 1] = mergeLazy(L[cur + 1], L[cur]);
            T[rc] = applyLazy(T[rc], getSegmentVal(L[cur], cR - m)); L[rc] = mergeLazy(L[rc], L[cur]);
            L[cur] = ldef;
        }
    }
    void build(int cur, int tl, int cR) {
        if (tl == cR) { T[cur] = A[tl]; L[cur] = ldef; return; }
        int m = tl + (cR - tl) / 2, rc = cur + (m - tl + 1) * 2; build(cur + 1, tl, m); build(rc, m + 1, cR);
        T[cur] = merge(T[cur + 1], T[rc]); L[cur] = ldef;
    }
    void update(int cur, int tl, int cR, int l, int r, const Lazy &val) {
        if (tl > r || cR < l) return;
        if (tl >= l && cR <= r) { T[cur] = applyLazy(T[cur], getSegmentVal(val, cR - tl + 1)); L[cur] = mergeLazy(L[cur], val); return; }
        int m = tl + (cR - tl) / 2, rc = cur + (m - tl + 1) * 2; propagate(cur, tl, cR);
        update(cur + 1, tl, m, l, r, val); update(rc, m + 1, cR, l, r, val); T[cur] = merge(T[cur + 1], T[rc]);
    }
    Data query(int cur, int tl, int cR, int l, int r) {
        if (tl > r || cR < l) return qdef;
        if (tl >= l && cR <= r) return T[cur];
        int m = tl + (cR - tl) / 2, rc = cur + (m - tl + 1) * 2; propagate(cur, tl, cR);
        return merge(query(cur + 1, tl, m, l, r), query(rc, m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 0; i < N; i++) A[i + ONE_INDEXED] = *(st + i);
        build(0, ONE_INDEXED, N - !ONE_INDEXED);
    }
    void init(int size) { N = size; fill(A + ONE_INDEXED, A + N + ONE_INDEXED, vdef); build(0, ONE_INDEXED, N - !ONE_INDEXED); }
    void update(int l, int r, const Lazy &val) { update(0, ONE_INDEXED, N - !ONE_INDEXED, l, r, val); }
    Data query(int l, int r) { return query(0, ONE_INDEXED, N - !ONE_INDEXED, l, r); }
};

using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0; const Lazy ldef = 0;
template <const int MAXNODES, const int MAXROOTS, const bool ONE_INDEXED> struct DynamicSegmentTree {
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int len); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    int N, curNode = 0, curRoot = 0, roots[MAXROOTS], L[MAXNODES], R[MAXNODES]; Data VAL[MAXNODES]; Lazy LZ[MAXNODES];
    int makeNode(int cp = 0) {
        assert(curNode < MAXNODES); L[curNode] = curNode ? L[cp] : 0; R[curNode] = curNode ? R[cp] : 0;
        VAL[curNode] = curNode ? VAL[cp] : vdef; LZ[curNode] = curNode ? LZ[cp] : ldef; return curNode++;
    }
    void propagate(int cur, int tl, int cR) {
        if (LZ[cur] != ldef) {
            int m = tl + (cR - tl) / 2;
            if (!L[cur]) L[cur] = makeNode();
            VAL[L[cur]] = applyLazy(VAL[L[cur]], getSegmentVal(LZ[cur], m - tl + 1)); LZ[L[cur]] = mergeLazy(LZ[L[cur]], LZ[cur]);
            if (!R[cur]) R[cur] = makeNode();
            VAL[R[cur]] = applyLazy(VAL[R[cur]], getSegmentVal(LZ[cur], cR - m)); LZ[R[cur]] = mergeLazy(LZ[R[cur]], LZ[cur]);
            LZ[cur] = ldef;
        }
    }
    template <class It> int build(int tl, int cR, It st) {
        int ret = makeNode();
        if (tl == cR) { VAL[ret] = *(st + tl - ONE_INDEXED); return ret; }
        int m = tl + (cR - tl) / 2; L[ret] = build(tl, m, st); R[ret] = build(m + 1, cR, st);
        VAL[ret] = merge(VAL[L[ret]], VAL[R[ret]]); return ret;
    }
    int update(int cur, int tl, int cR, int l, int r, const Lazy &val, bool persistent) {
        int ret = persistent ? makeNode(cur) : cur;
        if (tl >= l && cR <= r) {
            VAL[ret] = applyLazy(VAL[ret], getSegmentVal(val, cR - tl + 1)); LZ[ret] = mergeLazy(LZ[ret], val); return ret;
        }
        int m = tl + (cR - tl) / 2; propagate(ret, tl, cR);
        if (tl <= r && l <= m) L[ret] = update(L[ret] ? L[ret] : makeNode(), tl, m, l, r, val, persistent);
        if (m + 1 <= r && l <= cR) R[ret] = update(R[ret] ? R[ret] : makeNode(), m + 1, cR, l, r, val, persistent);
        if (L[ret] && R[ret]) VAL[ret] = merge(VAL[L[ret]], VAL[R[ret]]);
        else if (L[ret]) VAL[ret] = VAL[L[ret]];
        else if (R[ret]) VAL[ret] = VAL[R[ret]];
        return ret;
    }
    Data query(int cur, int tl, int cR, int l, int r) {
        if (tl > r || cR < l) return qdef;
        if (!cur) return vdef;
        if (tl >= l && cR <= r) return VAL[cur];
        int m = tl + (cR - tl) / 2; propagate(cur, tl, cR); return merge(query(L[cur], tl, m, l, r), query(R[cur], m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) { N = en - st; makeNode(); roots[curRoot++] = build(ONE_INDEXED, N - !ONE_INDEXED, st); }
    void init(int size) { N = size; makeNode(); roots[curRoot++] = makeNode(); }
    void update(int l, int r, const Lazy &val, bool persistent) {
        int nr = update(roots[curRoot - 1], ONE_INDEXED, N - !ONE_INDEXED, l, r, val, persistent);
        if (persistent) roots[curRoot++] = nr;
        else roots[curRoot - 1] = nr;
    }
    Data query(int l, int r, int rootInd = -1) {
        return query((rootInd == -1 ? roots[curRoot - 1] : roots[rootInd]), ONE_INDEXED, N - !ONE_INDEXED, l, r);
    }
    void revert(int x) { roots[curRoot++] = roots[x]; }
    void tlear() { curNode = curRoot = 0; }
};
