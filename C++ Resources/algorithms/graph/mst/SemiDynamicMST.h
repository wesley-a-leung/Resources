#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the minimum spanning tree after an edge has been added
// Online Link Cut Tree Solution
// Time Complexity: O((V + Q) log (V + Q))
// Memory Complexity: O(V + Q)

using unit = int; const unit NEG_INF = (numeric_limits<unit>::min)();
// Stripped down version of Link Cut Tree for maximum edge weight queries
using Data = pair<unit, int>; using Lazy = unit; const Data vdef = make_pair(NEG_INF, -1), qdef = make_pair(NEG_INF, -1);
Data merge(const Data &l, const Data &r) { return max(l, r); }
Data applyLazy(const Data &l, const Lazy &r) { return make_pair(r, l.second); }
void revData(Data &v) {}
struct Node {
    Node *l, *r, *p; int size; Data val, sbtr; bool rev;
    Node(const Data &val) : l(nullptr), r(nullptr), p(nullptr), size(1), val(val), sbtr(val), rev(false) {}
    bool isRoot(); void update(); void apply(const Lazy &v); void propagate(); void rotate();
    void splay(); Node *expose(); void makeRoot(); Node *findRoot();
};
int Size(Node *x) { return x ? x->size : 0; }
Data Sbtr(Node *x) { return x ? x->sbtr : qdef; }
bool Node::isRoot() { return !p || (this != p->l && this != p->r); }
void Node::update() {
    size = 1; sbtr = val;
    if (l) { size += l->size; sbtr = merge(l->sbtr, sbtr); }
    if (r) { size += r->size; sbtr = merge(sbtr, r->sbtr); }
}
void Node::apply(const Lazy &v) { val = applyLazy(val, v); sbtr = applyLazy(sbtr, v); }
void Node::propagate() {
    if (rev) {
        swap(l, r); rev = false;
        if (l) { l->rev = !l->rev; revData(l->sbtr); }
        if (r) { r->rev = !r->rev; revData(r->sbtr); }
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
struct LinkCutTree {
    vector<Node> T; int MAXNODES = 0;
    void reserve(int N) { T.reserve(N); MAXNODES = N; }
    void init(int N) { for (int i = 0; i < N; i++) { T.emplace_back(vdef); assert(int(T.size()) <= MAXNODES); } }
    void makeNode(int id, unit weight) { T.emplace_back(make_pair(weight, id)); assert(int(T.size()) <= MAXNODES); }
    bool connected(int x, int y) {
        if (x == y) return true;
        if (T[x].findRoot() != T[y].findRoot()) return false;
        T[x].expose(); T[y].expose(); return T[x].p;
    }
    bool link(int par, int ch) {
        T[ch].makeRoot(); T[ch].p = &T[par]; return true;
    }
    bool cut(int x, int y) {
        T[x].makeRoot(); T[y].expose();
        if (&T[x] != T[y].r || T[x].l) return false;
        T[y].r->p = nullptr; T[y].r = nullptr; return true;
    }
    void updateVertex(int x, const Lazy &val) { T[x].makeRoot(); T[x].apply(val); }
    Data queryPath(int from, int to) {
        T[from].makeRoot(); T[to].expose(); return Sbtr(&T[to]);
    }
    void clear() { T.clear(); MAXNODES = 0; }
};

struct SemiDynamicMST {
    struct Edge { int v, w; unit weight; };
    int V; LinkCutTree LCT; unit currentMST; vector<Edge> edges;
    void init(int V, int Q) { this->V = V; currentMST = 0; LCT.reserve(V + Q); LCT.init(V); }
    void clear() { LCT.clear(); edges.clear(); }
    unit addEdge(int v, int w, unit weight) {
        if (LCT.connected(v, w)) {
            pair<unit, int> mx = LCT.queryPath(v, w);
            if (mx.first <= weight) return currentMST;
            LCT.cut(edges[mx.second].v, V + mx.second); LCT.cut(edges[mx.second].w, V + mx.second); currentMST -= mx.first;
        }
        int id = int(edges.size()); edges.push_back({v, w, weight});
        LCT.makeNode(id, weight); LCT.link(v, V + id); LCT.link(w, V + id); currentMST += weight;
        return currentMST;
    }
};
