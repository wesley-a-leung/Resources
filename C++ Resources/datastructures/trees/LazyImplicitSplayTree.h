#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Splay Tree supporting range updates and queries
// Time Complexity:
//   constructor: O(N)
//   updateVal, queryVal, updateRange, queryRange, reverseRange: O(log N)
// Memory Complexity: O(N)

using Data = int; using Lazy = int; const Data vdef = 0; const Lazy ldef = 0;
Data merge(const Data &l, const Data &r); // to be implemented
Lazy getSegmentVal(const Lazy &v, int k); // to be implemented
Lazy mergeLazy(const Lazy &l, const Lazy &r); // to be implemented
Data applyLazy(const Data &l, const Lazy &r); // to be implemented
void revData(Data &v); // to be implemented
struct Node {
    Node *l, *r, *p; int size; Data val, sbtr; Lazy lz; bool rev;
    Node(const Data &val) : l(nullptr), r(nullptr), p(nullptr), size(1), val(val), sbtr(val), lz(ldef), rev(false) {}
    void update(); void apply(const Lazy &v); void propagate(); void rotate(Node *rootP); void splay(Node *rootP);
};
int Size(Node *x) { return x ? x->size : 0; }
Data Val(Node *x) { return x ? x->val : vdef; }
Data Sbtr(Node *x) { return x ? x->sbtr : vdef; }
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
void connect(Node *ch, Node *par, bool isL) {
    if (ch) ch->p = par;
    if (par) (isL ? par->l : par->r) = ch;
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
    propagate(); update();
}
struct LazyImplicitSplayTree {
    Node *root = nullptr; vector<Node> T;
    Node *select(Node *x, int k) {
        if (!x) return nullptr;
        x->propagate(); int t = Size(x->l);
        if (t > k) return select(x->l, k);
        else if (t < k) return select(x->r, k - t - 1);
        return x;
    }
    Node *build(int l, int r, const vector<Data> &A) {
        if (l > r) return nullptr;
        int m = l + (r - l) / 2, i = int(T.size()); T.emplace_back(A[m]);
        Node *left = build(l, m - 1, A), *right = build(m + 1, r, A), *x = &(T[i]);
        connect(left, x, true); connect(right, x, false); x->update();
        return x;
    }
    // [l, r] is at root->r->l
    void slice(int l, int r) { (root = select(root, l - 1))->splay(nullptr); select(root, r + 1)->splay(root); }
    void updateToRoot() { if (root->r->l) { root->r->l->propagate(); root->r->l->update(); } root->r->update(); root->update(); }
    // 1-indexed, inclusive
    void updateVal(int ind, const Lazy &val) { slice(ind, ind); root->r->l->apply(val); updateToRoot(); }
    void updateRange(int l, int r, const Lazy &val) {
        slice(l, r);
        if (root->r->l) root->r->l->apply(val);
        updateToRoot();
    }
    void reverseRange(int l, int r) {
        slice(l, r);
        if (root->r->l) { root->r->l->rev = !root->r->l->rev; revData(root->r->l->sbtr); }
        updateToRoot();
    }
    Data queryVal(int ind) { slice(ind, ind); return Val(root->r->l); }
    Data queryRange(int l, int r) { slice(l, r); return Sbtr(root->r->l); }
    LazyImplicitSplayTree(int N) { T.reserve(N + 2); vector<Data> A(N + 2, vdef); root = build(0, A.size() - 1, A); }
    template <class It> LazyImplicitSplayTree(It st, It en) {
        T.reserve(en - st + 2); vector<Data> A; A.push_back(vdef); A.insert(A.end(), st, en); A.push_back(vdef);
        root = build(0, A.size() - 1, A);
    }
};
