#pragma once
#include <bits/stdc++.h>
using namespace std;

// Link Cut Tree supporting path updates and path queries
// Time Complexity:
//   constructor: O(N)
//   makeRoot, findRoot, lca, link, cut, updateVertex, updatePath, queryPath: O(log N)
// Memory Complexity: O(N)

using Data = int; using Lazy = int; const Data vdef = 0, qdef = 0; const Lazy ldef = 0;
Data merge(const Data &l, const Data &r); // to be implemented
Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
void revData(Data &v); // to be implemented
struct Node {
    Node *l, *r, *p; int vert, size; Data val, sbtr; Lazy lz; bool rev;
    Node(int vert, const Data &val) : l(nullptr), r(nullptr), p(nullptr), vert(vert), size(1), val(val), sbtr(val), lz(ldef), rev(false) {}
    bool isRoot(); void update(); void apply(const Lazy &v); void propagate(); void rotate();
    void splay(); Node *expose(); void makeRoot(); Node *findRoot(); Node *findMin();
};
int Size(Node *x) { return x ? x->size : 0; }
Data Sbtr(Node *x) { return x ? x->sbtr : qdef; }
bool Node::isRoot() { return !p || (this != p->l && this != p->r); }
void Node::update() {
    size = 1; sbtr = val;
    if (l) { size += l->size; sbtr = merge(l->sbtr, sbtr); }
    if (r) { size += r->size; sbtr = merge(sbtr, r->sbtr); }
}
void Node::apply(const Lazy &v) { val = applyLazy(val, v); sbtr = applyLazy(sbtr, getSegmentVal(v, size)); lz = mergeLazy(lz, v); }
void Node::propagate() {
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
void connect(Node *ch, Node *par, bool hasCh, bool isL) {
    if (ch) ch->p = par;
    if (hasCh) (isL ? par->l : par->r) = ch;
}
void Node::rotate() {
    Node *p = this->p, *g = p->p; bool isRootP = p->isRoot(), isL = this == p->l;
    connect(isL ? r : l, p, true, isL); connect(p, this, true, !isL); connect(this, g, !isRootP, isRootP ? false : p == g->l); p->update();
}
void Node::splay() {
    while (!isRoot()) {
        Node *p = this->p, *g = p->p;
        if (!p->isRoot()) g->propagate();
        p->propagate(); propagate();
        if (!p->isRoot()) ((this == p->l) == (p == g->l) ? p : this)->rotate();
        rotate();
    }
    propagate(); update();
}
Node *Node::expose() {
    Node *last = nullptr;
    for (Node *y = this; y; y = y->p) { y->splay(); y->l = last; last = y; }
    splay(); return last;
}
void Node::makeRoot() { expose(); rev = !rev; revData(sbtr); }
Node *Node::findRoot() {
    expose(); Node *x = this;
    while (x->r) x = x->r;
    x->splay(); return x;
}
Node *Node::findMin() {
    Node *x = this;
    while (x->l) x = x->l;
    x->splay(); return x;
}
struct LinkCutTree {
    vector<Node> T;
    LinkCutTree(int N) { T.reserve(N); for (int i = 0; i < N; i++) T.emplace_back(i, vdef); }
    template <class It> LinkCutTree(It st, It en) { int N = en - st; T.reserve(N); for (int i = 0; i < N; i++) T.emplace_back(i, *(st + i)); }
    void makeRoot(int x) { T[x].makeRoot(); }
    bool connected(int x, int y) {
        if (x == y) return true;
        if (T[x].findRoot() != T[y].findRoot()) return false;
        T[x].expose(); T[y].expose(); return T[x].p;
    }
    int lca(int x, int y, int r) {
        if (T[x].findRoot() != T[y].findRoot()) return -1;
        T[r].makeRoot(); T[x].expose(); return T[y].expose()->vert;
    }
    bool link(int par, int ch) {
        if (connected(par, ch)) return false;
        T[ch].makeRoot(); T[ch].p = &T[par]; return true;
    }
    bool cut(int x, int y) {
        if (!connected(x, y)) return false;
        T[x].makeRoot(); T[y].expose();
        if (&T[x] != T[y].r || T[x].l) return false;
        T[y].r->p = nullptr; T[y].r = nullptr; return true;
    }
    bool cutParent(int ch) {
        T[ch].expose();
        if (!T[ch].r) return false;
        T[ch].r->p = nullptr; T[ch].r = nullptr; return true;
    }
    int findParent(int ch) { T[ch].expose(); return T[ch].r ? T[ch].r->findMin()->vert : -1; }
    void updateVertex(int x, const Lazy &val) { T[x].makeRoot(); T[x].apply(val); }
    bool updatePath(int from, int to, const Lazy &val) {
        if (!connected(from, to)) return false;
        T[from].makeRoot(); T[to].expose(); T[to].apply(val); return true;
    }
    Data queryPath(int from, int to) {
        if (!connected(from, to)) return qdef;
        T[from].makeRoot(); T[to].expose(); return Sbtr(&T[to]);
    }
};
