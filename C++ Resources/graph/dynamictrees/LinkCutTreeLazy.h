#pragma once
#include <bits/stdc++.h>
using namespace std;

// Link Cut Tree supporting path updates and path queries
// Time Complexity:
//   constructor: O(N)
//   makeRoot, findRoot, findParent, lca, link, cut, updateVertex, updatePath, queryPath: O(log N)
// Memory Complexity: O(N)

using Data = int; using Lazy = int; const Lazy ldef = 0;
Data merge(const Data &l, const Data &r); // to be implemented
Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
void revData(Data &v); // to be implemented
struct Node {
    Node *l, *r, *p; int vert, size; Data val, sbtr; Lazy lz; bool rev;
    Node(int vert, const Data &val) : l(nullptr), r(nullptr), p(nullptr), vert(vert), size(1), val(val), sbtr(val), lz(ldef), rev(false) {}
    bool isRoot() { return !p || (this != p->l && this != p->r); }
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
    friend void connect(Node *ch, Node *par, bool hasCh, bool isL) {
        if (ch) ch->p = par;
        if (hasCh) (isL ? par->l : par->r) = ch;
    }
    void rotate() {
        Node *p = this->p, *g = p->p; bool isRootP = p->isRoot(), isL = this == p->l;
        connect(isL ? r : l, p, true, isL); connect(p, this, true, !isL); connect(this, g, !isRootP, isRootP ? false : p == g->l); p->update();
    }
    void splay() {
        while (!isRoot()) {
            Node *p = this->p, *g = p->p;
            if (!p->isRoot()) g->propagate();
            p->propagate(); propagate();
            if (!p->isRoot()) ((this == p->l) == (p == g->l) ? p : this)->rotate();
            rotate();
        }
        propagate(); update();
    }
    Node *access() {
        Node *last = nullptr;
        for (Node *y = this; y; y = y->p) { y->splay(); y->l = last; last = y; }
        splay(); return last;
    }
    void makeRoot() { access(); rev = !rev; revData(sbtr); }
    Node *findMin() {
        Node *x = this;
        for (x->propagate(); x->l; (x = x->l)->propagate());
        x->splay(); return x;
    }
    Node *findMax() {
        Node *x = this;
        for (x->propagate(); x->r; (x = x->r)->propagate());
        x->splay(); return x;
    }
};
struct LinkCutTreeLazy {
    vector<Node> T;
    LinkCutTreeLazy(int N, const Data &vdef) { T.reserve(N); for (int i = 0; i < N; i++) T.emplace_back(i, vdef); }
    template <class It> LinkCutTreeLazy(It st, It en) { int N = en - st; T.reserve(N); for (int i = 0; i < N; i++) T.emplace_back(i, *(st + i)); }
    void makeRoot(int x) { T[x].makeRoot(); }
    bool connected(int x, int y) {
        if (x == y) return true;
        T[x].access(); T[y].access(); return T[x].p;
    }
    // lca, link, updatePath, updatePath, queryPath do not check for connectedness
    int lca(int x, int y) { T[x].access(); return T[y].access()->vert; }
    void link(int x, int y) { T[y].makeRoot(); T[y].p = &T[x]; }
    bool cut(int x, int y) {
        T[x].makeRoot(); T[y].access();
        if (&T[x] != T[y].r || T[x].l) return false;
        T[y].r->p = nullptr; T[y].r = nullptr; return true;
    }
    bool cutParent(int ch) {
        T[ch].access();
        if (!T[ch].r) return false;
        T[ch].r->p = nullptr; T[ch].r = nullptr; return true;
    }
    int findParent(int ch) { T[ch].access(); return T[ch].r ? T[ch].r->findMin()->vert : -1; }
    int findRoot(int x) { T[x].access(); return T[x].findMax()->vert; }
    void updateVertex(int x, const Lazy &val) { T[x].makeRoot(); T[x].apply(val); }
    void updatePath(int from, int to, const Lazy &val) { T[from].makeRoot(); T[to].access(); T[to].apply(val); }
    Data queryPath(int from, int to) { T[from].makeRoot(); T[to].access(); return T[to].sbtr; }
};
