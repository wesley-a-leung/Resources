#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Data {
    int mn, mx, sum, size; Data(int mn = INT_MAX, int mx = INT_MIN, int sum = 0, int size = 0) : mn(mn), mx(mx), sum(sum), size(size) {}
};
struct Lazy {
    int lz, fl; Lazy(int lz = 0, int fl = 0): lz(lz), fl(fl) {}
};

// Top Tree supporting path and subtree, updates and queries
// Time Complexity:
//   constructor: O(N)
//   makeRoot, findParent, link, cutParent: O(log N)
//   updatePath, updateSubtree, queryPath, querySubtree: O(log N)
// Memory Complexity: O(N)
struct TopTree {
    const int INC = 1, ASSIGN = 2;
    Data merge(const Data &l, const Data &r) {
        return Data(min(l.mn, r.mn), max(l.mx, r.mx), l.sum + r.sum, l.size + r.size);
    }
    int apply(int val, int lz, int fl) {
        if (fl == ASSIGN) return lz;
        else if (fl == INC) return val + lz;
        else return val;
    }
    Data applyLazy(const Data &d, const Lazy &lz) {
        return d.size ? Data(apply(d.mn, lz.lz, lz.fl), apply(d.mx, lz.lz, lz.fl), apply(d.sum, lz.lz * d.size, lz.fl), d.size) : d;
    }
    Lazy mergeLazy(const Lazy &ch, const Lazy &par) {
        return Lazy(apply(ch.lz, par.lz, par.fl), max(ch.fl, par.fl));
    }
    vector<Data> PATH, SBTR, FTR; vector<Lazy> LZPATH, LZSBTR;
    vector<array<int, 4>> CH; vector<int> VAL, P; vector<bool> REV, INR;
    int makeNode(int val, bool inr) {
        VAL.push_back(val); Data d = inr ? Data() : Data(val, val, val, 1); Lazy lz = Lazy(0, 0);
        PATH.push_back(d); SBTR.push_back(d); FTR.push_back(d); LZPATH.push_back(lz); LZSBTR.push_back(lz);
        CH.push_back({0, 0, 0, 0}); P.push_back(0); REV.push_back(false); INR.push_back(inr);
        return int(VAL.size()) - 1;
    }
    TopTree(int N) { makeNode(0, false); for (int i = 1; i <= N; i++) makeNode(0, false); }
    template <class It> TopTree(It st, It en) { makeNode(0, false); for (It i = st; i < en; i++) makeNode(*i, false); }
    bool isRoot(int x, bool t) {
        if (t) return !P[x] || !INR[x] || !INR[P[x]];
        else return !P[x] || (x != CH[P[x]][0] && x != CH[P[x]][1]);
    }
    void rev(int x) { REV[x] = !REV[x]; swap(CH[x][0], CH[x][1]); }
    void pushPath(int x, const Lazy &lz) {
        LZPATH[x] = mergeLazy(LZPATH[x], lz); PATH[x] = applyLazy(PATH[x], lz);
        VAL[x] = apply(VAL[x], lz.lz, lz.fl); SBTR[x] = merge(PATH[x], FTR[x]);
    }
    void pushSbtr(int x, const Lazy &lz, bool ftr) {
        LZSBTR[x] = mergeLazy(LZSBTR[x], lz); SBTR[x] = applyLazy(SBTR[x], lz); FTR[x] = applyLazy(FTR[x], lz);
        if (ftr) pushPath(x, lz);
    }
    void propagate(int x) {
        if (x) {
            if (REV[x]) {
                REV[x] = false;
                for (int i = 0; i < 2; i++) if (CH[x][i]) rev(CH[x][i]);
            }
            for (int i = 0; i < 4; i++) if (CH[x][i]) pushSbtr(CH[x][i], LZSBTR[x], i >= 2);
            for (int i = 0; i < 2; i++) if (CH[x][i]) pushPath(CH[x][i], LZPATH[x]);
            LZSBTR[x] = LZPATH[x] = Lazy();
        }
    }
    void update(int x) {
        if (x) {
            PATH[x] = SBTR[x] = FTR[x] = Data();
            if (!INR[x]) PATH[x] = SBTR[x] = Data(VAL[x], VAL[x], VAL[x], 1);
            for (int i = 0; i < 2; i++) if (CH[x][i]) { PATH[x] = merge(PATH[x], PATH[CH[x][i]]); FTR[x] = merge(FTR[x], FTR[CH[x][i]]); }
            for (int i = 0; i < 4; i++) if (CH[x][i]) SBTR[x] = merge(SBTR[x], SBTR[CH[x][i]]);
            for (int i = 2; i < 4; i++) if (CH[x][i]) FTR[x] = merge(FTR[x], SBTR[CH[x][i]]);
        }
    }
    int findInd(int x) {
        for (int i = 0; i < 4; i++) if (CH[P[x]][i] == x) return i;
        assert(false); return -1;
    }
    void connect(int ch, int par, int i) {
        if (ch) P[ch] = par;
        CH[par][i] = ch;
    }
    void rotate(int x, int t) {
        assert(t == 0 || t == 2); int p = P[x], g = P[p]; bool isL = x == CH[p][t];
        if (g) connect(x, g, findInd(p));
        else P[x] = 0;
        connect(CH[x][t ^ isL], p, t ^ !isL); connect(p, x, t ^ isL); update(p);
    }
    void splay(int x, int t) {
        assert(t == 0 || t == 2);
        while (!isRoot(x, t)) {
            int p = P[x], g = P[p];
            if (!isRoot(p, t)) rotate((x == CH[p][t]) == (p == CH[g][t]) ? p : x, t);
            rotate(x, t);
        }
        update(x);
    }
    void add(int x, int y) {
        propagate(y);
        for (int i = 2; i < 4; i++) if (!CH[y][i]) { connect(x, y, i); return; }
        int z = makeNode(0, true), w = 0;
        for(w = y; INR[CH[w][2]]; w = CH[w][2]) propagate(CH[w][2]);
        connect(CH[w][2], z, 2); connect(x, z, 3); connect(z, w, 2); splay(z, 2);
    }
    void remove(int x) {
        if (INR[P[x]]) { connect(CH[P[x]][findInd(x) ^ 1], P[P[x]], findInd(P[x])); splay(P[P[x]], 2); }
        else connect(0, P[x], findInd(x));
        P[x] = 0;
    }
    void expose(int x) {
        stack<int> stk; int y = x, z;
        for (z = x; z; z = P[z]) stk.push(z);
        while (!stk.empty()) { propagate(stk.top()); stk.pop(); }
        splay(x, 0);
        if (CH[x][1]) { z = CH[x][1]; CH[x][1] = 0; add(z, x); update(x); }
        while (P[x]) {
            for (z = P[x]; INR[z]; z = P[z]);
            splay(z, 0);
            if (CH[z][1]) { connect(CH[z][1], P[x], findInd(x)); splay(P[x], 2); }
            else remove(x);
            connect(x, z, 1); update(z); x = z;
        }
        splay(y, 0);
    }
    void makeRoot(int x) { expose(x); rev(x); }
    int findParent(int x) {
        expose(x); propagate(CH[x][0]);
        for (x = CH[x][0]; x && CH[x][1]; x = CH[x][1]) propagate(CH[x][1]);
        return x;
    }
    int findRoot(int x) { for (; P[x]; x = P[x]); return x; }
    int cutParent(int x) {
        int y = findParent(x);
        if (y) { expose(y); remove(x); update(y); }
        return y;
    }
    void link(int x, int y) {
        makeRoot(x); int p = cutParent(x);
        if (findRoot(x) != findRoot(y)) p = y;
        if (p) { expose(p); add(x, p); update(p); }
    }
    void updateSubtree(int x, Lazy lz) {
        expose(x); VAL[x] = apply(VAL[x], lz.lz, lz.fl);
        for (int i = 2; i < 4; i++) if (CH[x][i]) pushSbtr(CH[x][i], lz, true);
        update(x);
    }
    void updatePath(int x, int y, Lazy lz) { makeRoot(x); expose(y); splay(x, 0); pushPath(x, lz); }
    Data querySubtree(int x) {
        expose(x); Data ret = (VAL[x], VAL[x], VAL[x], 1);
        for (int i = 2; i < 4; i++) ret = merge(ret, SBTR[CH[x][i]]);
        return ret;
    }
    Data queryPath(int x, int y) { makeRoot(x); expose(y); splay(x, 0); return PATH[x]; }
};
