#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Splay Tree supporting range updates and queries
// Time Complexity:
//   constructor: O(N)
//   updateVal, queryVal, updateRange, queryRange, reverseRange: O(log N) amortized
// Memory Complexity: O(N)

using Data = int; using Lazy = int; const Data qdef = 0; const Lazy ldef = 0;
Data merge(const Data &l, const Data &r); // to be implemented
Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
void revData(Data &v); // to be implemented
struct Node {
    Node *l, *r, *p; int size; Data val, sbtr; Lazy lz; bool rev;
    Node(const Data &val) : l(nullptr), r(nullptr), p(nullptr), size(1), val(val), sbtr(val), lz(ldef), rev(false) {}
    void update() {
        size = 1; sbtr = val;
        if (l) { size += l->size; sbtr = merge(l->sbtr, sbtr); }
        if (r) { size += r->size; sbtr = merge(sbtr, r->sbtr); }
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
    static void connect(Node *ch, Node *par, bool isL) {
        if (ch) ch->p = par;
        if (par) (isL ? par->l : par->r) = ch;
    }
    void rotate() {
        Node *p = this->p, *g = p->p; bool isRootP = !g, isL = this == p->l;
        connect(isL ? r : l, p, isL); connect(p, this, !isL); connect(this, g, isRootP ? false : p == g->l); p->update();
    }
    void splay() {
        while (p) {
            Node *p = this->p, *g = p->p;
            if (g) g->propagate();
            p->propagate(); propagate();
            if (g) ((this == p->l) == (p == g->l) ? p : this)->rotate();
            rotate();
        }
        propagate(); update();
    }
};
int Size(Node *x) { return x ? x->size : 0; }
Data Sbtr(Node *x) { return x ? x->sbtr : qdef; }
Node *_select(Node *x, int k) {
    if (!x) return nullptr;
    x->propagate(); int t = Size(x->l);
    if (t > k) return _select(x->l, k);
    else if (t < k) return _select(x->r, k - t - 1);
    return x;
}
Node *select(Node *&root, Node *x, int k) { // 0-indexed
    if ((x = _select(x, k))) (root = x)->splay();
    return x;
}
int index(Node *&root, Node *x) { // 0-indexed
    if (!x) return -1;
    (root = x)->splay(); return Size(x->l);
}
void merge(Node *&x, Node *l, Node *r) {
    if (!l || !r) { x = l ? l : r; }
    else { select(x, l, Size(l) - 1)->splay(); Node::connect(r, x, false); x->update(); }
}
void split(Node *x, Node *&l, Node *&r, int lsz) {
    if (!x) { l = r = nullptr; }
    else if (lsz == 0) { l = nullptr; r = x; }
    else if (lsz == Size(x)) { l = x; r = nullptr; }
    else { select(l, x, lsz - 1); r = l->r; Node::connect(nullptr, l, false); Node::connect(r, nullptr, false); l->update(); r->propagate(); r->update(); }
}
struct ImplicitSplayTreeLazy {
    vector<Node> T; Node *root;
    ImplicitSplayTreeLazy(int N, const Data &vdef) {
        T.reserve(N); root = nullptr;
        for (int i = 0; i < N; i++) { T.emplace_back(vdef); merge(root, root, &T.back()); }
    }
    template <class It> ImplicitSplayTreeLazy(It st, It en) {
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
