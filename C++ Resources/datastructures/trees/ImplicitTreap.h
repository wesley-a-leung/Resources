#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Treap supporting point updates and range queries
// Time Complexity:
//   constructor: O(N)
//   updateVal, queryVal, queryRange: O(log N)
// Memory Complexity: O(N)

seed_seq seq {
    (uint64_t)chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
mt19937_64 rng64(seq); uniform_int_distribution<long long> dis;
using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0;
Data merge(const Data &l, const Data &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
struct Node {
    Node *l, *r; int size; long long pri; Data val, sbtr;
    Node (const Data &val) : l(nullptr), r(nullptr), size(1), pri(dis(rng64)), val(val), sbtr(val) {}
    void update() {
        size = 1; sbtr = val;
        if (l) { size += l->size; sbtr = merge(l->sbtr, sbtr); }
        if (r) { size += r->size; sbtr = merge(sbtr, r->sbtr); }
    }
    void apply(const Lazy &v) { val = applyLazy(val, v); sbtr = applyLazy(sbtr, v); }
};
int Size(Node *x) { return x ? x->size : 0; }
Data Val(Node *x) { return x ? x->val : vdef; }
Data Sbtr(Node *x) { return x ? x->sbtr : qdef; }
void merge(Node *&x, Node *l, Node *r) {
    if (!l || !r) { x = l ? l : r; }
    else if (l->pri > r->pri) { merge(l->r, l->r, r); x = l; }
    else { merge(r->l, l, r->l); x = r; }
    if (x) x->update();
}
void split(Node *x, Node *&l, Node *&r, int lsz) {
    if (!x) { l = r = nullptr; return; }
    if (lsz <= Size(x->l)) { split(x->l, l, x->l, lsz); r = x; }
    else { split(x->r, x->r, r, lsz - Size(x->l) - 1); l = x; }
    x->update();
}
struct ImplicitTreap {
    vector<Node> T; Node *root;
    ImplicitTreap(int N) {
        T.reserve(N); root = nullptr;
        for (int i = 0; i < N; i++) { T.emplace_back(vdef); merge(root, root, &T.back()); }
    }
    template <class It> ImplicitTreap(It st, It en) {
        int N = en - st; T.reserve(N); root = nullptr;
        for (int i = 0; i < N; i++) { T.emplace_back(*(st + i)); merge(root, root, &T.back()); }
    }
    // 0-indexed, inclusive
    void updateVal(int i, const Lazy &val) {
        Node *left, *mid, *right; split(root, left, mid, i); split(mid, mid, right, 1);
        mid->apply(val); merge(root, left, mid); merge(root, root, right);
    }
    Data queryVal(int i) {
        Node *left, *mid, *right; split(root, left, mid, i); split(mid, mid, right, 1);
        Data ret = Val(mid); merge(root, left, mid); merge(root, root, right); return ret;
    }
    Data queryRange(int l, int r) {
        Node *left, *mid, *right; split(root, left, mid, l); split(mid, mid, right, r - l + 1);
        Data ret = Sbtr(mid); merge(root, left, mid); merge(root, root, right); return ret;
    }
};
