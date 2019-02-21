#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Treap supporting range updates and queries
// Time Complexity:
//   constructor: O(N)
//   updateRange, queryRange: O(log N)
// Memory Complexity: O(N)
struct LazyImplicitTreap {
    seed_seq seq {
        (uint64_t) chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
        (uint64_t) __builtin_ia32_rdtsc(),
        (uint64_t) (uintptr_t) make_unique<char>().get()
    };
    mt19937 rng; uniform_real_distribution<double> dis;
    using Data = int; using Lazy = int; const Data vdef = 0; const Lazy ldef = 0;
    vector<Data> VAL, SBTR; vector<Lazy> LZ; vector<int> L, R, SZ; vector<double> PRI; int root = -1;
    int makeNode(const Data &val) {
        VAL.push_back(val); SBTR.push_back(val); LZ.push_back(ldef);
        L.push_back(-1); R.push_back(-1); SZ.push_back(1); PRI.push_back(dis(rng));
        return int(VAL.size()) - 1;
    }
    int size(int x) { return x == -1 ? 0 : SZ[x]; }
    Data sbtrVal(int x) { return x == -1 ? vdef : SBTR[x]; }
    Data merge(const Data &l, const Data &r); // to be implemented
    Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
    Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
    Data applyLazy(const Data &l, const Lazy &r); // to be implemented
    void apply(int x, const Lazy &v) {
        if (x == -1) return;
        VAL[x] = applyLazy(VAL[x], v); SBTR[x] = applyLazy(SBTR[x], getSegmentVal(v, SZ[x])); LZ[x] = mergeLazy(LZ[x], v);
    }
    void propagate(int x) {
        if (x == -1 || LZ[x] == ldef) return;
        if (L[x] != -1) apply(L[x], LZ[x]);
        if (R[x] != -1) apply(R[x], LZ[x]);
        LZ[x] = ldef;
    }
    void update(int x) {
        if (x == -1) return;
        SZ[x] = 1; SBTR[x] = VAL[x];
        if (L[x] != -1) { SZ[x] += SZ[L[x]]; SBTR[x] = merge(SBTR[L[x]], SBTR[x]); }
        if (R[x] != -1) { SZ[x] += SZ[R[x]]; SBTR[x] = merge(SBTR[x], SBTR[R[x]]); }
    }
    void merge(int &x, int l, int r) {
        propagate(l); propagate(r);
        if (l == -1 || r == -1) { x = l == -1 ? r : l; }
        else if (PRI[l] > PRI[r]) { merge(R[l], R[l], r); x = l; }
        else { merge(L[r], l, L[r]); x = r; }
        update(x);
    }
    void split(int x, int &l, int &r, int lsz) {
        if (x == -1) { l = r = -1; return; }
        propagate(x);
        if (lsz <= size(L[x])) { split(L[x], l, L[x], lsz); r = x; }
        else { split(R[x], R[x], r, lsz - size(L[x]) - 1); l = x; }
        update(x);
    }
    LazyImplicitTreap(int N) : rng(seq), dis(0.0, 1.0) {
        for (int i = 0; i < N; i++) merge(root, root, makeNode(vdef));
    }
    template <class It> LazyImplicitTreap(It st, It en) : rng(seq), dis(0.0, 1.0) {
        for (It i = st; i < en; i++) merge(root, root, makeNode(*i));
    }
    // 0-indexed, inclusive
    void updateRange(int l, int r, const Lazy &val) {
        int left, right, mid; split(root, left, mid, l); split(mid, mid, right, r - l + 1);
        apply(mid, val); merge(root, left, mid); merge(root, root, right);
    }
    Data queryRange(int l, int r) {
        int left, right, mid; split(root, left, mid, l); split(mid, mid, right, r - l + 1);
        Data ret = sbtrVal(mid); merge(root, left, mid); merge(root, root, right);
        return ret;
    }
};
