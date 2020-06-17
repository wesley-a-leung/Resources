#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the minimum spanning tree after an edge has been added
// Online Link Cut Tree Solution
// Time Complexity: O((V + Q) log (V + Q))
// Memory Complexity: O(V + Q)

using unit = int; const unit NEG_INF = numeric_limits<unit>::lowest();
// Stripped down version of Link Cut Tree for maximum edge weight queries
using Data = pair<unit, int>; const Data qdef = make_pair(NEG_INF, -1);
Data merge(const Data &l, const Data &r) { return max(l, r); }
struct Node {
    Node *l, *r, *p; Data val, sbtr; bool rev;
    Node(const Data &val) : l(nullptr), r(nullptr), p(nullptr), val(val), sbtr(val), rev(false) {}
    bool isRoot() { return !p || (this != p->l && this != p->r); }
    void update() {
        sbtr = val;
        if (l) sbtr = merge(l->sbtr, sbtr);
        if (r) sbtr = merge(sbtr, r->sbtr);
    }
    void propagate() {
        if (rev) {
            swap(l, r); rev = false;
            if (l) l->rev = !l->rev;
            if (r) r->rev = !r->rev;
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
    Node *expose() {
        Node *last = nullptr;
        for (Node *y = this; y; y = y->p) { y->splay(); y->l = last; last = y; }
        splay(); return last;
    }
    void makeRoot() { expose(); rev = !rev; }
};
struct SemiDynamicMST {
    struct Edge { int v, w; unit weight; };
    int V, MAXNODES = 0; vector<Node> T; unit currentMST; vector<Edge> edges;
    void makeNode(int id, unit weight) { T.emplace_back(make_pair(weight, id)); assert(int(T.size()) <= MAXNODES); }
    bool connected(int x, int y) {
        if (x == y) return true;
        T[x].expose(); T[y].expose(); return T[x].p;
    }
    void link(int par, int ch) { T[ch].makeRoot(); T[ch].p = &T[par]; }
    void cut(int x, int y) { T[x].makeRoot(); T[y].expose(); T[y].r->p = nullptr; T[y].r = nullptr; }
    Data queryPath(int from, int to) { T[from].makeRoot(); T[to].expose(); return T[to].sbtr; }
    void init(int V, int Q) {
        this->V = V; currentMST = 0; T.reserve(MAXNODES = V + Q);
        for (int i = 0; i < V; i++) { T.emplace_back(make_pair(NEG_INF, -1)); }
    }
    void clear() { MAXNODES = 0; T.clear(); edges.clear(); }
    unit addEdge(int v, int w, unit weight) {
        if (connected(v, w)) {
            pair<unit, int> mx = queryPath(v, w);
            if (mx.first <= weight) return currentMST;
            cut(edges[mx.second].v, V + mx.second); cut(edges[mx.second].w, V + mx.second); currentMST -= mx.first;
        }
        int id = int(edges.size()); edges.push_back({v, w, weight}); makeNode(id, weight);
        link(v, V + id); link(w, V + id); currentMST += weight; return currentMST;
    }
};
