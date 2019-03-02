#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the minimum spanning tree after an edge is added or removed
// Offline Divide and Conquer Solution
// Time Complexity: O(V + E * (Q / B) + Q * B)
// Memory Complexity: O(V + E + Q)
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
    bool connected(int x, int y) { return x == y || T[x].findRoot() == T[y].findRoot(); }
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

template <const int MAXV> struct DynamicMSTDivAndConq {
    struct Query { int type, v, w; unit weight; int otherTime; };
    int V; LinkCutTree LCT; unit currentMST; vector<unit> ans; vector<Query> q;
    unordered_map<int, int> present[MAXV]; stack<pair<pair<int, int>, unit>> history;
    void init(int V, int Q) { this->V = V; currentMST = 0; LCT.reserve(V + Q); LCT.init(V); }
    void clear(int V = MAXV) { LCT.clear(); ans.clear(); q.clear(); for (int i = 0; i < V; i++) present[i].clear(); }
    void link(int i) {
        int v = q[i].v, w = q[i].w; unit weight = q[i].weight;
        if (LCT.connected(v, w)) {
            pair<unit, int> mx = LCT.queryPath(v, w);
            if (mx.first <= weight) return;
            history.emplace(make_pair(i, mx.second), currentMST);
            LCT.cut(q[mx.second].v, V + mx.second); LCT.cut(q[mx.second].w, V + mx.second); currentMST -= mx.first;
        } else history.emplace(make_pair(i, -1), currentMST);
        LCT.link(v, V + i); LCT.link(w, V + i); currentMST += weight;
    }
    void undo() {
        int i = history.top().first.first; int j = history.top().first.second; currentMST = history.top().second; history.pop();
        LCT.cut(q[i].v, V + i); LCT.cut(q[i].w, V + i);
        if (j != -1) { LCT.link(q[j].v, V + j); LCT.link(q[j].w, V + j); }
    }
    void solve(int l, int r) {
        if (l == r && q[l].type == 0) ans.push_back(currentMST);
        if (l >= r) return;
        int m = l + (r - l) / 2, curSize = history.size();
        for (int i = m + 1; i <= r; i++) if (q[i].otherTime < l) link(i);
        solve(l, m);
        while (int(history.size()) > curSize) undo();
        for (int i = l; i <= m; i++) if (q[i].otherTime > r) link(i);
        solve(m + 1, r);
        while (int(history.size()) > curSize) undo();
    }
    void addEdge(int v, int w, unit weight) {
        if (v > w) swap(v, w);
        present[v][w] = int(q.size()); LCT.makeNode(int(q.size()), weight);
        q.push_back({1, v, w, weight, INT_MAX});
    }
    void removeEdge(int v, int w) {
        if (v > w) swap(v, w);
        int insTime = present[v][w]; LCT.makeNode(int(q.size()), q[insTime].weight);
        q[insTime].otherTime = int(q.size()); q.push_back({-1, v, w, q[insTime].weight, insTime});
    }
    void query() { LCT.makeNode(int(q.size()), 0); q.push_back({0, -1, -1, 0, int(q.size())}); }
    void solve() { solve(0, int(q.size()) - 1); }
};
