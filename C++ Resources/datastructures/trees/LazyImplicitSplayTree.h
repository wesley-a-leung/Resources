#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Splay Tree supporting range updates and queries
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)

using Data = int; using Lazy = int; const Data vdef = 0; const Lazy ldef = 0;
struct Node {
    Node *l, *r, *p; int size; Data val, sbtr; Lazy lz;
    Node(Data val) : l(nullptr), r(nullptr), p(nullptr), size(1), val(val), sbtr(val), lz(ldef) {}
    void apply(Lazy lz);
    void propagate();
    void update();
    void rotate(Node *rootP);
    void splay(Node *rootP);
};
int Size(Node *x) { return x ? x->size : 0; }
Data Sbtr(Node *x) { return x ? x->sbtr : vdef; }
Data merge(Data l, Data r) { return l + r; } // to be implemented
Lazy getSegmentVal(Lazy v, int k); // to be implemented
Lazy mergeLazy(Lazy l, Lazy r); // to be implemented
Data applyLazy(Data d, Lazy l); // to be implemented
void Node::apply(Lazy l) { val = applyLazy(val, l); sbtr = applyLazy(sbtr, getSegmentVal(l, size)); lz = mergeLazy(lz, l); }
void Node::propagate() {
    if (lz != ldef) {
        if (l) l->apply(lz);
        if (r) r->apply(lz);
        lz = ldef;
    }
}
void Node::update() {
    size = 1; sbtr = val;
    if (l) { size += Size(l); sbtr = merge(sbtr, l->sbtr); }
    if (r) { size += Size(r); sbtr = merge(sbtr, r->sbtr); }
}
void connect(Node *ch, Node *par, bool isL) {
    if (ch) ch->p = par;
    if (par) {
        if (isL) par->l = ch;
        else par->r = ch;
    }
}
void Node::rotate(Node *rootP) {
    Node *p = this->p, *g = p->p; bool isRootP = g == rootP, isL = this == p->l;
    connect(isL ? r : l, p, isL); connect(p, this, !isL); connect(this, g, isRootP ? false : p == g->l); p->update();
}
void Node::splay(Node *rootP) {
    while (p != rootP) {
        Node *p = this->p, *g = p->p;
        if (g != rootP) g->propagate();
        p->propagate(); propagate();
        if (g != rootP) ((this == p->l) == (p == g->l) ? p : this)->rotate(rootP);
        rotate(rootP);
    }
    update();
}
struct LazyImplicitSplayTree {
    Node *root = nullptr; int cur = 0; vector<Node> T;
    Node *select(Node *x, int k) {
        if (!x) return nullptr;
        int t = Size(x->l);
        if (t > k) return select(x->l, k);
        else if (t < k) return select(x->r, k - t - 1);
        return x;
    }
    Node *build(int l, int r, vector<Data> &A) {
        if (l > r) return nullptr;
        int m = l + (r - l) / 2, i = cur; T[cur++] = Node(A[m]);
        Node *left = build(l, m - 1, A), *right = build(m + 1, r, A), *x = &(T[i]);
        connect(left, x, true); connect(right, x, false); x->update();
        return x;
    }
    //  [l, r] is at root->r->l
    void slice(int l, int r) { (root = select(root, l - 1))->splay(nullptr); select(root, r + 1)->splay(root); }
    void updateToRoot() { if (root->r->l) { root->r->l->propagate(); root->r->l->update(); root->r->update(); root->update(); } }
    void update(int l, int r, Lazy val) {
        slice(l, r);
        if (root->r->l) root->r->l->apply(val);
        updateToRoot();
    }
    Data query(int l, int r) { slice(l, r); return Sbtr(root->r->l); }
    LazyImplicitSplayTree(int N) { T.resize(N + 2); vector<Data> A(N + 2, vdef); root = build(0, A.size() - 1, A); }
    template <class It> LazyImplicitSplayTree(It st, It en) {
        T.resize(en - st + 2); vector<Data> A; A.push_back(vdef); A.insert(A.back(), st, en); A.push_back(vdef);
        root = build(0, A.size() - 1, A);
    }
};
