#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Splay Tree supporting point updates and range queries
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)

using Data = int; const Data vdef = 0;
struct Node {
    Node *l, *r, *p; int size; Data val, sbtr;
    Node() {}
    Node(Data val) : l(nullptr), r(nullptr), p(nullptr), size(1), val(val), sbtr(val) {}
    void update();
    void rotate(Node *rootP);
    void splay(Node *rootP);
};
int Size(Node *x) { return x ? x->size : 0; }
Data Sbtr(Node *x) { return x ? x->sbtr : vdef; }
Data merge(Data l, Data r) { return l + r; } // to be implemented
Data applyVal(Data o, Data n) { return o + n; } // to be implemented
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
        if (g != rootP) ((this == p->l) == (p == g->l) ? p : this)->rotate(rootP);
        rotate(rootP);
    }
    update();
}
template <const int MAXN> struct ImplicitSplayTree {
    Node T[MAXN], *root = nullptr; int cur = 0;
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
    void updateToRoot() { if (root->r->l) { root->r->l->update(); root->r->update(); root->update(); } }
    void update(int ind, Data val) { slice(ind, ind); root->r->l->val = applyVal(root->r->l->val, val); updateToRoot(); }
    Data query(int l, int r) { slice(l, r); return Sbtr(root->r->l); }
    void init(int N) { vector<Data> A(N + 2, vdef); root = build(0, int(A.size()) - 1, A); }
    template <class It> ImplicitSplayTree(It st, It en) {
        vector<Data> A; A.push_back(vdef); A.insert(A.back(), st, en); A.push_back(vdef);
        root = build(0, int(A.size()) - 1, A);
    }
};
