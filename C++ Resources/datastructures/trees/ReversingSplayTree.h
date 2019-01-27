#pragma once
#include <bits/stdc++.h>
using namespace std;

// Implicit Splay Tree supporting point queries and range reversals
// Time Complexity:
//   constructor: O(N)
//   reverseRange, getValue: O(log N)
// Memory Complexity: O(N)

using Data = int; const Data vdef = 0;
struct Node {
    Node *l, *r, *p; int size; Data val; bool rev;
    Node(Data val) : l(nullptr), r(nullptr), p(nullptr), size(1), val(val), rev(false) {}
    void propagate();
    void update();
    void rotate(Node *rootP);
    void splay(Node *rootP);
};
int Size(Node *x) { return x ? x->size : 0; }
void Node::propagate() {
    if (rev) {
        swap(l, r);
        if (l) l->rev = !l->rev;
        if (r) r->rev = !r->rev;
        rev = false;
    }
}
void Node::update() {
    size = 1;
    if (l) size += Size(l);
    if (r) size += Size(r);
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
    propagate(); update();
}
struct ReversingSplayTree {
    Node *root = nullptr; int cur = 0; vector<Node> T;
    Node *select(Node *x, int k) {
        if (!x) return nullptr;
        x->propagate(); int t = Size(x->l);
        if (t > k) return select(x->l, k);
        else if (t < k) return select(x->r, k - t - 1);
        return x;
    }
    Node *build(int l, int r, vector<Data> &A) {
        if (l > r) return nullptr;
        int m = l + (r - l) / 2, i = int(T.size()); T.emplace_back(A[m]);
        Node *left = build(l, m - 1, A), *right = build(m + 1, r, A), *x = &(T[i]);
        connect(left, x, true); connect(right, x, false); x->update();
        return x;
    }
    //  [l, r] is at root->r->l
    void slice(int l, int r) { (root = select(root, l - 1))->splay(nullptr); select(root, r + 1)->splay(root); }
    void updateToRoot() { if (root->r->l) { root->r->l->propagate(); root->r->l->update(); root->r->update(); root->update(); } }
    // 1-indexed, inclusive
    void reverseRange(int l, int r) {
        slice(l, r);
        if (root->r->l) root->r->l->rev = !root->r->l->rev;
        updateToRoot();
    }
    Data getValue(int ind) { slice(ind, ind); return root->r->l->val; }
    ReversingSplayTree(int N) { T.reserve(N + 2); vector<Data> A(N + 2, vdef); root = build(0, A.size() - 1, A); }
    template <class It> ReversingSplayTree(It st, It en) {
        T.reserve(en - st + 2); vector<Data> A; A.push_back(vdef); A.insert(A.end(), st, en); A.push_back(vdef);
        root = build(0, A.size() - 1, A);
    }
};
