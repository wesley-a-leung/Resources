#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Treap supporting point queries and range reversals
// Time Complexity:
//   constructor: O(N)
//   reverseRange, getValue: O(log N)
// Memory Complexity: O(N)
struct ReversingImplicitTreap {
    seed_seq seq {
        (uint64_t) chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
        (uint64_t) __builtin_ia32_rdtsc(),
        (uint64_t) (uintptr_t) make_unique<char>().get()
    };
    mt19937 rng; uniform_real_distribution<double> dis;
    using Data = int; const Data vdef = 0;
    vector<Data> VAL; vector<bool> REV; vector<int> L, R, SZ; vector<double> PRI; int root = -1;
    int makeNode(const Data &val) {
        VAL.push_back(val); REV.push_back(false); L.push_back(-1); R.push_back(-1); SZ.push_back(1); PRI.push_back(dis(rng));
        return int(VAL.size()) - 1;
    }
    int size(int x) { return x == -1 ? 0 : SZ[x]; }
    void propagate(int x) {
        if (x == -1 || !REV[x]) return;
        swap(L[x], R[x]);
        if (L[x] != -1) REV[L[x]] = !REV[L[x]];
        if (R[x] != -1) REV[R[x]] = !REV[R[x]];
        REV[x] = false;
    }
    void update(int x) {
        if (x == -1) return;
        SZ[x] = 1;
        if (L[x] != -1) SZ[x] += SZ[L[x]];
        if (R[x] != -1) SZ[x] += SZ[R[x]];
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
    ReversingImplicitTreap(int N) : rng(seq), dis(0.0, 1.0) {
        for (int i = 0; i < N; i++) merge(root, root, makeNode(vdef));
    }
    template <class It> ReversingImplicitTreap(It st, It en) : rng(seq), dis(0.0, 1.0) {
        for (It i = st; i < en; i++) merge(root, root, makeNode(*i));
    }
    // 0-indexed, inclusive
    void reverseRange(int l, int r) {
        int left, right, mid; split(root, left, mid, l); split(mid, mid, right, r - l + 1);
        REV[mid] = !REV[mid]; merge(root, left, mid); merge(root, root, right);
    }
    Data getValue(int ind) {
        int left, right, mid; split(root, left, mid, ind); split(mid, mid, right, 1);
        Data ret = VAL[mid]; merge(root, left, mid); merge(root, root, right); return ret;
    }
};
