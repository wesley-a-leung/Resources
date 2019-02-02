#pragma once
#include <bits/stdc++.h>
using namespace std;

template <const int MAXN, const bool ONE_INDEXED> struct SegmentTree {
    using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0;
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    Data T[MAXN * 4], A[MAXN]; int N;
    void build(int cur, int cL, int cR) {
        if (cL == cR) { T[cur] = A[cL]; return; }
        int m = cL + (cR - cL) / 2; build(cur * 2, cL, m); build(cur * 2 + 1, m + 1, cR);
        T[cur] = merge(T[cur * 2], T[cur * 2 + 1]);
    }
    void update(int cur, int cL, int cR, int ind, const Lazy &val) {
        if (cL > ind || cR < ind) return;
        if (cL == cR) { T[cur] = applyLazy(T[cur], val); return; }
        int m = cL + (cR - cL) / 2; update(cur * 2, cL, m, ind, val); update(cur * 2 + 1, m + 1, cR, ind, val);
        T[cur] = merge(T[cur * 2], T[cur * 2 + 1]);
    }
    Data query(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return qdef;
        if (cL >= l && cR <= r) return T[cur];
        int m = cL + (cR - cL) / 2;
        return merge(query(cur * 2, cL, m, l, r), query(cur * 2 + 1, m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 0; i < N; i++) A[i + ONE_INDEXED] = *(st + i);
        build(1, ONE_INDEXED, N - !ONE_INDEXED);
    }
    void init(int size) { N = size; fill(A + ONE_INDEXED, A + N + ONE_INDEXED, vdef); build(1, ONE_INDEXED, N - !ONE_INDEXED); }
    void update(int ind, const Lazy &val) { update(1, ONE_INDEXED, N - !ONE_INDEXED, ind, val); }
    Data query(int l, int r) { return query(1, ONE_INDEXED, N - !ONE_INDEXED, l, r); }
};

template <const int MAXN, const bool ONE_INDEXED> struct LazySegmentTree {
    using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0; const Lazy ldef = 0;
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int len); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    Data T[MAXN * 4], A[MAXN]; Lazy L[MAXN * 4]; int N;
    void propagate(int cur, int cL, int cR) {
        if (L[cur] != ldef) {
            int m = cL + (cR - cL) / 2;
            T[cur * 2] = applyLazy(T[cur * 2], getSegmentVal(L[cur], m - cL + 1)); L[cur * 2] = mergeLazy(L[cur * 2], L[cur]);
            T[cur * 2 + 1] = applyLazy(T[cur * 2 + 1], getSegmentVal(L[cur], cR - m)); L[cur * 2 + 1] = mergeLazy(L[cur * 2 + 1], L[cur]);
            L[cur] = ldef;
        }
    }
    void build(int cur, int cL, int cR) {
        if (cL == cR) { T[cur] = A[cL]; L[cur] = ldef; return; }
        int m = cL + (cR - cL) / 2; build(cur * 2, cL, m); build(cur * 2 + 1, m + 1, cR);
        T[cur] = merge(T[cur * 2], T[cur * 2 + 1]); L[cur] = ldef;
    }
    void update(int cur, int cL, int cR, int l, int r, const Lazy &val) {
        if (cL > r || cR < l) return;
        if (cL >= l && cR <= r) {
            T[cur] = applyLazy(T[cur], getSegmentVal(val, cR - cL + 1));
            L[cur] = mergeLazy(L[cur], val); return;
        }
        int m = cL + (cR - cL) / 2; propagate(cur, cL, cR);
        update(cur * 2, cL, m, l, r, val); update(cur * 2 + 1, m + 1, cR, l, r, val); T[cur] = merge(T[cur * 2], T[cur * 2 + 1]);
    }
    Data query(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return qdef;
        if (cL >= l && cR <= r) return T[cur];
        int m = cL + (cR - cL) / 2; propagate(cur, cL, cR);
        return merge(query(cur * 2, cL, m, l, r), query(cur * 2 + 1, m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) {
        N = en - st;
        for (int i = 0; i < N; i++) A[i + ONE_INDEXED] = *(st + i);
        build(1, ONE_INDEXED, N - !ONE_INDEXED);
    }
    void init(int size) { N = size; fill(A + ONE_INDEXED, A + N + ONE_INDEXED, vdef); build(1, ONE_INDEXED, N - !ONE_INDEXED); }
    void update(int l, int r, const Lazy &val) { update(1, ONE_INDEXED, N - !ONE_INDEXED, l, r, val); }
    Data query(int l, int r) { return query(1, ONE_INDEXED, N - !ONE_INDEXED, l, r); }
};

using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0; const Lazy ldef = 0;
template <const int MAXNODES, const int MAXROOTS, const bool ONE_INDEXED> struct DyanmicSegmentTree {
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int len); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    int N, curNode = 0, curRoot = 0, roots[MAXROOTS], L[MAXNODES], R[MAXNODES]; Data VAL[MAXNODES]; Lazy LZ[MAXNODES];
    int makeNode(int cp = 0) {
        assert(curNode < MAXNODES); L[curNode] = curNode ? L[cp] : 0; R[curNode] = curNode ? R[cp] : 0;
        VAL[curNode] = curNode ? VAL[cp] : vdef; LZ[curNode] = curNode ? LZ[cp] : ldef; return curNode++;
    }
    void propagate(int cur, int cL, int cR) {
        if (LZ[cur] != ldef) {
            int m = cL + (cR - cL) / 2;
            if (!L[cur]) L[cur] = makeNode();
            VAL[L[cur]] = applyLazy(VAL[L[cur]], getSegmentVal(LZ[cur], m - cL + 1)); LZ[L[cur]] = mergeLazy(LZ[L[cur]], LZ[cur]);
            if (!R[cur]) R[cur] = makeNode();
            VAL[R[cur]] = applyLazy(VAL[R[cur]], getSegmentVal(LZ[cur], cR - m)); LZ[R[cur]] = mergeLazy(LZ[R[cur]], LZ[cur]);
            LZ[cur] = ldef;
        }
    }
    template <class It> int build(int cL, int cR, It st) {
        int ret = makeNode();
        if (cL == cR) { VAL[ret] = *(st + cL - ONE_INDEXED); return ret; }
        int m = cL + (cR - cL) / 2; L[ret] = build(cL, m, st); R[ret] = build(m + 1, cR, st);
        VAL[ret] = merge(VAL[L[ret]], VAL[R[ret]]); return ret;
    }
    int update(int cur, int cL, int cR, int l, int r, const Lazy &val, bool persistent) {
        if (cL > r || cR < l) return cur;
        int ret = persistent ? makeNode(cur) : cur;
        if (cL >= l && cR <= r) {
            VAL[ret] = applyLazy(VAL[ret], getSegmentVal(val, cR - cL + 1));
            LZ[ret] = mergeLazy(LZ[ret], val); return ret;
        }
        if (!L[ret]) L[ret] = makeNode();
        if (!R[ret]) R[ret] = makeNode();
        int m = cL + (cR - cL) / 2; propagate(ret, cL, cR);
        L[ret] = update(L[ret], cL, m, l, r, val, persistent);
        R[ret] = update(R[ret], m + 1, cR, l, r, val, persistent);
        if (L[ret] && R[ret]) VAL[ret] = merge(VAL[L[ret]], VAL[R[ret]]);
        else if (L[ret]) VAL[ret] = merge(VAL[L[ret]], Data(vdef));
        else if (R[ret]) VAL[ret] = merge(VAL[R[ret]], Data(vdef));
        return ret;
    }
    Data query(int cur, int cL, int cR, int l, int r) {
        if (cL > r || cR < l) return qdef;
        if (!cur) return vdef;
        if (cL >= l && cR <= r) return VAL[cur];
        int m = cL + (cR - cL) / 2; propagate(cur, cL, cR); return merge(query(L[cur], cL, m, l, r), query(R[cur], m + 1, cR, l, r));
    }
    template <class It> void init(It st, It en) { N = en - st; makeNode(); roots[curRoot++] = build(ONE_INDEXED, N - !ONE_INDEXED, st); }
    void init(int size) { N = size; makeNode(); roots[curRoot++] = makeNode(); }
    void update(int l, int r, Lazy val, bool persistent) {
        int nr = update(roots[curRoot - 1], ONE_INDEXED, N - !ONE_INDEXED, l, r, val, persistent);
        if (persistent) roots[curRoot++] = nr;
        else roots[curRoot - 1] = nr;
    }
    Data query(int l, int r, int rootInd = -1) {
        return query((rootInd == -1 ? roots[curRoot - 1] : roots[rootInd]), ONE_INDEXED, N - !ONE_INDEXED, l, r);
    }
    void revert(int x) { roots[curRoot++] = roots[x]; }
    void clear() { curNode = curRoot = 0; }
};
