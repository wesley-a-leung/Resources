#pragma once
#include <bits/stdc++.h>
using namespace std;

// Euler Tour Treap supporting vertex updates and vertex/subtree queries
// Time Complexity:
//   constructor: O(N)
//   treeRoot, connected, addEdge, cutParent: O(log N)
//   updateVertex, queryVertexValue, querySubtreeValue: O(log N)
// Memory Complexity: O(N)
struct EulerTourTreap {
    seed_seq seq {
        (uint64_t) chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
        (uint64_t) __builtin_ia32_rdtsc(),
        (uint64_t) (uintptr_t) make_unique<char>().get()
    };
    mt19937 rng; uniform_real_distribution<double> dis;
    using Data = int; using Lazy = int; const Data vdef = 0; const bool ISPRE = true, ISPOST = false;
    vector<Data> VAL, SBTR; vector<int> PRE, POST, VERT, L, R, P, SZ; vector<double> PRI; vector<bool> TYPE;
    int makeNode(int vert, bool type, const Data &val) {
        VAL.push_back(val); SBTR.push_back(val); VERT.push_back(vert); TYPE.push_back(type);
        L.push_back(-1); R.push_back(-1); P.push_back(-1); SZ.push_back(1); PRI.push_back(dis(rng));
        return int(VAL.size()) - 1;
    }
    int size(int x) { return x == -1 ? 0 : SZ[x]; }
    Data val(int x) { return x == -1 ? vdef : VAL[x]; }
    Data sbtrVal(int x) { return x == -1 ? vdef : SBTR[x]; }
    Data merge(const Data &l, const Data &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    void apply(int x, const Lazy &v) {
        if (x == -1) return;
        VAL[x] = applyLazy(VAL[x], v); SBTR[x] = applyLazy(SBTR[x], v);
    }
    void update(int x) {
        if (x == -1) return;
        SZ[x] = 1; SBTR[x] = VAL[x];
        if (L[x] != -1) { P[L[x]] = x; SZ[x] += SZ[L[x]]; SBTR[x] = merge(SBTR[x], SBTR[L[x]]); }
        if (R[x] != -1) { P[R[x]] = x; SZ[x] += SZ[R[x]]; SBTR[x] = merge(SBTR[x], SBTR[R[x]]); }
    }
    void merge(int &x, int l, int r) {
        if (l == -1 || r == -1) { x = l == -1 ? r : l; }
        else if (PRI[l] > PRI[r]) { merge(R[l], R[l], r); x = l; }
        else { merge(L[r], l, L[r]); x = r; }
        update(x);
    }
    void split(int x, int &l, int &r, int lsz) {
        if (x == -1) { l = r = -1; return; }
        P[x] = -1;
        if (lsz <= size(L[x])) { split(L[x], l, L[x], lsz); r = x; }
        else { split(R[x], R[x], r, lsz - size(L[x]) - 1); l = x; }
        update(x);
    }
    int min(int x) {
        if (x == -1) return 0;
        while (L[x] != -1) x = L[x];
        return x;
    }
    int root(int x) {
        if (x == -1) return 0;
        while (P[x] != -1) x = P[x];
        return x;
    }
    int index(int x) {
        int ind = size(L[x]);
        for (; P[x] != -1; x = P[x]) if (L[P[x]] != x) ind += 1 + size(L[P[x]]);
        return ind;
    }
    EulerTourTreap(int N) : rng(seq), dis(0.0, 1.0) {
        for (int i = 0, dummy = -1; i < N; i++) {
            PRE.push_back(makeNode(i, ISPRE, vdef)); POST.push_back(makeNode(i, ISPOST, vdef));
            merge(dummy, PRE.back(), POST.back());
        }
    }
    template <class It> EulerTourTreap(It st, It en) : rng(seq), dis(0.0, 1.0) {
        int dummy = -1;
        for (It i = st; i < en; i++) {
            PRE.push_back(makeNode(i, ISPRE, *i)); POST.push_back(makeNode(i, ISPOST, *i));
            merge(dummy, PRE.back(), POST.back());
        }
    }
    int treeRoot(int v) { return VERT[min(root(PRE[v]))]; }
    bool connected(int v, int w) { return treeRoot(v) == treeRoot(w); }
    void addEdge(int v, int w) {
        int l, r; split(root(PRE[v]), l, r, index(PRE[v]) + 1);
        merge(l, l, root(PRE[w])); merge(l, l, r);
    }
    void cutParent(int v) {
        int l, m, r; split(root(PRE[v]), l, m, index(PRE[v])); split(m, m, r, index(POST[v]) + 1);
        merge(l, l, r);
    }
    Data getVertexValue(int v) {
        int l, m, r; split(root(PRE[v]), l, m, index(PRE[v])); split(m, m, r, 1);
        Data ret = val(m); merge(l, l, m); merge(l, l, r);
        return ret;
    }
    Data getSubtreeValue(int v) { // value may be doubled due to double counting of pre and post
        int l, m, r; split(root(PRE[v]), l, m, index(PRE[v])); split(m, m, r, index(POST[v]) + 1);
        Data ret = sbtrVal(m); merge(l, l, m); merge(l, l, r);
        return ret;
    }
    void updateVertex(int v, const Lazy &val) {
        int l, preV, m, postV, r; split(root(PRE[v]), l, preV, index(PRE[v])); split(preV, preV, m, 1);
        split(m, m, postV, index(POST[v])); split(postV, postV, r, 1);
        apply(preV, val); apply(postV, val); merge(l, l, preV); merge(l, l, m); merge(l, l, postV); merge(l, l, r);
    }
};
