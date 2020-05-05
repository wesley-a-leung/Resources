#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Treap supporting range updates and queries
// Time Complexity:
//   constructor: O(N log N)
//   updateVal, queryVal, updateRange, queryRange, reverseRange: O(log N) expected
// Memory Complexity: O(N)

std::seed_seq seq{
    (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(),
    (uint64_t)__builtin_ia32_rdtsc(),(uint64_t)(uintptr_t)make_unique<char>().get()
};
std::mt19937_64 rng64(seq);
uniform_int_distribution<long long> dis;
using Data = int; using Lazy = int; const Data qdef = 0; const Lazy ldef = 0;
Data merge(const Data &l, const Data &r); // to be implemented
Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
void revData(Data &v); // to be implemented
struct Node {
    Node *l, *r, *p;
    int size; long long pri; Data val, sbtr; Lazy lz; bool rev;
    Node(const Data &val) : l(nullptr), r(nullptr), p(nullptr), size(1), pri(dis(rng64)), val(val), sbtr(val), lz(ldef), rev(false) {}
    void update() {
        size = 1; sbtr = val;
        if (l) { l->p = this; size += l->size; sbtr = merge(l->sbtr, sbtr); }
        if (r) { r->p = this; size += r->size; sbtr = merge(sbtr, r->sbtr); }
    }
    void apply(const Lazy &v) { val = applyLazy(val, v); sbtr = applyLazy(sbtr, getSegmentVal(v, size)); lz = mergeLazy(lz, v); }
    void propagate() {
        if (rev) {
            swap(l, r); rev = false;
            if (l) { l->rev = !l->rev; revData(l->sbtr); }
            if (r) { r->rev = !r->rev; revData(r->sbtr); }
        }
        if (lz != ldef) {
            if (l) l->apply(lz);
            if (r) r->apply(lz);
            lz = ldef;
        }
    }
};
int Size(Node *x) { return x ? x->size : 0; }
Data Sbtr(Node *x) { return x ? x->sbtr : qdef; }
void merge(Node *&x, Node *l, Node *r) {
    if (l) l->propagate();
    if (r) r->propagate();
    if (!l || !r) { x = l ? l : r; }
    else if (l->pri > r->pri) { merge(l->r, l->r, r); x = l; }
    else { merge(r->l, l, r->l); x = r; }
    if (x) x->update();
}
void split(Node *x, Node *&l, Node *&r, int lsz) {
    if (!x) { l = r = nullptr; return; }
    x->propagate(); x->p = nullptr;
    if (lsz <= Size(x->l)) { split(x->l, l, x->l, lsz); r = x; }
    else { split(x->r, x->r, r, lsz - Size(x->l) - 1); l = x; }
    x->update();
}
Node *select(Node *x, int k) { // 0-indexed
    if (!x) return nullptr;
    x->propagate(); int t = Size(x->l);
    if (t > k) return select(x->l, k);
    else if (t < k) return select(x->r, k - t - 1);
    return x;
}
int index(Node *x, Node *ch = nullptr) { // 0-indexed
    if (!x) return ch ? 0 : -1;
    int ind = index(x->p, x); x->propagate();
    if (!ch) return ind + Size(x->l);
    else if (x->l == ch) return ind;
    return ind + 1 + Size(x->l);
}
struct ImplicitTreapLazy {
    vector<Node> T; Node *root;
    ImplicitTreapLazy(int N, const Data &vdef) {
        T.reserve(N); root = nullptr;
        for (int i = 0; i < N; i++) { T.emplace_back(vdef); merge(root, root, &T.back()); }
    }
    template <class It> ImplicitTreapLazy(It st, It en) {
        int N = en - st; T.reserve(N); root = nullptr;
        for (int i = 0; i < N; i++) { T.emplace_back(*(st + i)); merge(root, root, &T.back()); }
    }
    // 0-indexed, inclusive
    void updateVal(int i, const Lazy &val) {
        Node *l, *m, *r; split(root, l, m, i); split(m, m, r, 1);
        m->apply(val); merge(root, l, m); merge(root, root, r);
    }
    Data queryVal(int i) {
        Node *l, *m, *r; split(root, l, m, i); split(m, m, r, 1);
        Data ret = m->val; merge(root, l, m); merge(root, root, r); return ret;
    }
    void updateRange(int a, int b, const Lazy &val) {
        Node *l, *m, *r; split(root, l, m, a); split(m, m, r, b - a + 1);
        if (m) m->apply(val);
        merge(root, l, m); merge(root, root, r);
    }
    Data queryRange(int a, int b) {
        Node *l, *m, *r; split(root, l, m, a); split(m, m, r, b - a + 1);
        Data ret = Sbtr(m); merge(root, l, m); merge(root, root, r); return ret;
    }
    void reverseRange(int a, int b) {
        Node *l, *m, *r; split(root, l, m, a); split(m, m, r, b - a + 1);
        if (m) { m->rev = !m->rev; revData(m->sbtr); }
        merge(root, l, m); merge(root, root, r);
    }
};
