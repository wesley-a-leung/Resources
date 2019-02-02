#pragma once
#include <bits/stdc++.h>
using namespace std;

// Link Cut Tree supporting vertex updates and path queries
// Time Complexity:
//   constructor: O(N)
//   makeRoot, findRoot, lca, link, cut, cutParent, updateVertex, queryPath: O(log N)
// Memory Complexity: O(N)
struct LinkCutTree {
    using Data = int; using Lazy = int; const Data vdef = 0;
    vector<Data> VAL, SBTR; vector<int> L, R, P, SZ; vector<bool> REV;
    int makeNode(const Data &val) {
        VAL.push_back(val); SBTR.push_back(val); REV.push_back(false);
        L.push_back(-1); R.push_back(-1); P.push_back(-1); SZ.push_back(1);
        return int(VAL.size()) - 1;
    }
    LinkCutTree(int N) { for (int i = 0; i < N; i++) makeNode(vdef); }
    template <class It> LinkCutTree(It st, It en) { for (It i = st; i < en; i++) makeNode(*i); }
    bool isRoot(int x) { return P[x] == -1 || (x != L[P[x]] && x != R[P[x]]); }
    int size(int x) { return x == -1 ? 0 : SZ[x]; }
    Data sbtr(int x) { return x == -1 ? vdef : SBTR[x]; }
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    void apply(int x, const Lazy &v) {
        if (x == -1) return;
        VAL[x] = applyLazy(VAL[x], v); SBTR[x] = applyLazy(SBTR[x], v);
    }
    void propagate(int x) {
        if (REV[x]) {
            REV[x] = false; swap(L[x], R[x]);
            if (L[x] != -1) REV[L[x]] = !REV[L[x]];
            if (R[x] != -1) REV[R[x]] = !REV[R[x]];
        }
    }
    void update(int x) { SBTR[x] = merge(merge(sbtr(L[x]), VAL[x]), sbtr(R[x])); SZ[x] = 1 + size(L[x]) + size(R[x]); }
    void connect(int ch, int par, bool hasCh, bool isL) {
        if (ch != -1) P[ch] = par;
        if (hasCh) {
            if (isL) L[par] = ch;
            else R[par] = ch;
        }
    }
    void rotate(int x) {
        int p = P[x], g = P[p]; bool isRootP = isRoot(p), isL = x == L[p];
        connect(isL ? R[x] : L[x], p, true, isL); connect(p, x, true, !isL); connect(x, g, !isRootP, isRootP ? false : p == L[g]);
        update(p);
    }
    void splay(int x) {
        while (!isRoot(x)) {
            int p = P[x], g = P[p];
            if (!isRoot(p)) propagate(g);
            propagate(p); propagate(x);
            if (!isRoot(p)) rotate((x == L[p]) == (p == L[g]) ? p : x);
            rotate(x);
        }
        propagate(x); update(x);
    }
    int expose(int x) {
        int last = -1;
        for (int y = x; y != -1; y = P[y]) { splay(y); L[y] = last; last = y; }
        splay(x); return last;
    }
    void makeRoot(int x) { expose(x); REV[x] = !REV[x]; }
    int findRoot(int x) {
        expose(x);
        while (R[x] != -1) x = R[x];
        splay(x); return x;
    }
    bool connected(int x, int y) {
        if (findRoot(x) != findRoot(y)) return false;
        if (x == y) return true;
        expose(x); expose(y); return P[x] != -1;
    }
    int lca(int x, int y, int r) {
        if (findRoot(x) != findRoot(y)) return -1;
        makeRoot(r); expose(x); return expose(y);
    }
    bool link(int x, int y) {
        if (connected(x, y)) return false;
        makeRoot(x); P[x] = y; return true;
    }
    bool cut(int x, int y) {
        if (!connected(x, y)) return false;
        makeRoot(x); expose(y);
        if (x != R[y] || L[x] != -1) return false;
        R[y] = P[R[y]] = -1; return true;
    }
    bool cutParent(int x) {
        expose(x);
        if (R[x] == -1) return false;
        R[x] = P[R[x]] = -1;
        return true;
    }
    void updateVertex(int x, const Lazy &val) { makeRoot(x); apply(x, val); }
    int queryPath(int from, int to) {
        if (!connected(from, to)) return vdef;
        makeRoot(from); expose(to); return sbtr(to);
    }
};
