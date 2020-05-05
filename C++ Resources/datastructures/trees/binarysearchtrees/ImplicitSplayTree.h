#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Splay Tree supporting point updates and range queries
// Time Complexity:
//   constructor: O(N)
//   updateVal, queryVal, queryRange: O(log N) amortized
// Memory Complexity: O(N)

using Data = int; using Lazy = int; const Data qdef = 0;
Data merge(const Data &l, const Data &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
struct Node {
    Node *l, *r, *p; int size; Data val, sbtr;
    Node(const Data &val) : l(nullptr), r(nullptr), p(nullptr), size(1), val(val), sbtr(val) {}
    void update() {
        size = 1; sbtr = val;
        if (l) { size += l->size; sbtr = merge(l->sbtr, sbtr); }
        if (r) { size += r->size; sbtr = merge(sbtr, r->sbtr); }
    }
    void apply(const Lazy &v) { val = applyLazy(val, v); sbtr = applyLazy(sbtr, v); }
    friend void connect(Node *ch, Node *par, bool isL) {
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
            if (g) ((this == p->l) == (p == g->l) ? p : this)->rotate();
            rotate();
        }
        update();
    }
};
int Size(Node *x) { return x ? x->size : 0; }
Data Sbtr(Node *x) { return x ? x->sbtr : qdef; }
Node *_select(Node *x, int k) {
    if (!x) return nullptr;
    int t = Size(x->l);
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
    else { select(x, l, Size(l) - 1)->splay(); connect(r, x, false); x->update(); }
}
void split(Node *x, Node *&l, Node *&r, int lsz) {
    if (!x) { l = r = nullptr; }
    else if (lsz == 0) { l = nullptr; r = x; }
    else if (lsz == Size(x)) { l = x; r = nullptr; }
    else { select(l, x, lsz - 1); r = l->r; connect(nullptr, l, false); connect(r, nullptr, false); l->update(); r->update(); }
}
struct ImplicitSplayTree {
    vector<Node> T; Node *root;
    ImplicitSplayTree(int N, const Data &vdef) {
        T.reserve(N); root = nullptr;
        for (int i = 0; i < N; i++) { T.emplace_back(vdef); merge(root, root, &T.back()); }
    }
    template <class It> ImplicitSplayTree(It st, It en) {
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
    Data queryRange(int a, int b) {
        Node *l, *m, *r; split(root, l, m, a); split(m, m, r, b - a + 1);
        Data ret = Sbtr(m); merge(root, l, m); merge(root, root, r); return ret;
    }
};
