#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for the minimum spanning tree after an edge is added or removed
// Offline Divide and Conquer Solution
// Time Complexity: O(V + Q log Q log (V + E))
// Memory Complexity: O(V + E + Q)

using unit = int; const unit NEG_INF = (numeric_limits<unit>::min)();
// Stripped down version of Link Cut Tree for maximum edge weight queries
using Data = pair<unit, int>; const Data vdef = make_pair(NEG_INF, -1), qdef = make_pair(NEG_INF, -1);
Data merge(const Data &l, const Data &r) { return max(l, r); }
struct Node {
    Node *l, *r, *p; Data val, sbtr; bool rev;
    Node(const Data &val) : l(nullptr), r(nullptr), p(nullptr), val(val), sbtr(val), rev(false) {}
    bool isRoot(); void update(); void propagate(); void rotate(); void splay(); Node *expose(); void makeRoot();
};
Data Sbtr(Node *x) { return x ? x->sbtr : qdef; }
bool Node::isRoot() { return !p || (this != p->l && this != p->r); }
void Node::update() {
    sbtr = val;
    if (l) sbtr = merge(l->sbtr, sbtr);
    if (r) sbtr = merge(sbtr, r->sbtr);
}
void Node::propagate() {
    if (rev) {
        swap(l, r); rev = false;
        if (l) l->rev = !l->rev;
        if (r) r->rev = !r->rev;
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
void Node::makeRoot() { expose(); rev = !rev; }
template <const int MAXV> struct DynamicMSTDivAndConq {
    struct Query { int type, v, w; unit weight; int otherTime; };
    int V, MAXNODES = 0; vector<Node> T; unit currentMST; vector<unit> ans; vector<Query> q;
    unordered_map<int, int> present[MAXV]; vector<pair<pair<int, int>, unit>> history;
    void makeNode(int id, unit weight) { T.emplace_back(make_pair(weight, id)); assert(int(T.size()) <= MAXNODES); }
    bool connected(int x, int y) {
        if (x == y) return true;
        T[x].expose(); T[y].expose(); return T[x].p;
    }
    void link(int par, int ch) { T[ch].makeRoot(); T[ch].p = &T[par]; }
    void cut(int x, int y) { T[x].makeRoot(); T[y].expose(); T[y].r->p = nullptr; T[y].r = nullptr; }
    Data queryPath(int from, int to) { T[from].makeRoot(); T[to].expose(); return Sbtr(&T[to]); }
    void init(int V, int Q) {
        this->V = V; currentMST = 0; T.reserve(MAXNODES = V + Q);
        for (int i = 0; i < V; i++) T.emplace_back(vdef);
    }
    void clear(int V = MAXV) { MAXNODES = 0; T.clear(); ans.clear(); q.clear(); for (int i = 0; i < V; i++) present[i].clear(); }
    void add(int i) {
        int v = q[i].v, w = q[i].w; unit weight = q[i].weight;
        if (connected(v, w)) {
            pair<unit, int> mx = queryPath(v, w);
            if (mx.first <= weight) return;
            history.emplace_back(make_pair(i, mx.second), currentMST);
            cut(q[mx.second].v, V + mx.second); cut(q[mx.second].w, V + mx.second); currentMST -= mx.first;
        } else history.emplace_back(make_pair(i, -1), currentMST);
        link(v, V + i); link(w, V + i); currentMST += weight;
    }
    void undo() {
        int i = history.back().first.first; int j = history.back().first.second; currentMST = history.back().second;
        history.pop_back(); cut(q[i].v, V + i); cut(q[i].w, V + i);
        if (j != -1) { link(q[j].v, V + j); link(q[j].w, V + j); }
    }
    void solve(int l, int r) {
        if (l == r && q[l].type == 0) ans.push_back(currentMST);
        if (l >= r) return;
        int m = l + (r - l) / 2, curSize = history.size();
        for (int i = m + 1; i <= r; i++) if (q[i].otherTime < l) add(i);
        solve(l, m);
        while (int(history.size()) > curSize) undo();
        for (int i = l; i <= m; i++) if (q[i].otherTime > r) add(i);
        solve(m + 1, r);
        while (int(history.size()) > curSize) undo();
    }
    void addEdge(int v, int w, unit weight) {
        if (v > w) swap(v, w);
        present[v][w] = int(q.size()); makeNode(int(q.size()), weight); q.push_back({1, v, w, weight, INT_MAX});
    }
    void removeEdge(int v, int w) {
        if (v > w) swap(v, w);
        int insTime = present[v][w]; makeNode(int(q.size()), q[insTime].weight);
        q[insTime].otherTime = int(q.size()); q.push_back({-1, q[insTime].v,  q[insTime].w, q[insTime].weight, insTime});
    }
    void query() { makeNode(int(q.size()), 0); q.push_back({0, -1, -1, 0, int(q.size())}); }
    void solve() { solve(0, int(q.size()) - 1); }
};
