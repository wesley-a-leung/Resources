#pragma once
#include <bits/stdc++.h>
using namespace std;

// Support queries for the number of components in a graph, after edges have been added or removed
// Link Cut Tree Solution
// Time Complexity: O(V + Q * log(V + Q))
// Memory Complexity: O(V + Q)

// Stripped down version of Link Cut Tree for minimum edge weight queries
using Data = pair<int, int>; const Data vdef = make_pair(INT_MAX, -1), qdef = make_pair(INT_MAX, -1);
Data merge(const Data &l, const Data &r) { return min(l, r); }
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
template <const int MAXV, const int MAXQ> struct DynamicConnectivityLCT {
    int Q = 0, cnt; vector<Node> T; vector<int> ans; unordered_map<int, int> present[MAXV];
    struct Query { int type, v, w, otherTime; } q[MAXQ];
    bool connected(int x, int y) {
        if (x == y) return true;
        T[x].expose(); T[y].expose(); return T[x].p;
    }
    void link(int par, int ch) { T[ch].makeRoot(); T[ch].p = &T[par]; }
    void cut(int x, int y) { T[x].makeRoot(); T[y].expose(); T[y].r->p = nullptr; T[y].r = nullptr; }
    Data queryPath(int from, int to) { T[from].makeRoot(); T[to].expose(); return Sbtr(&T[to]); }
    void clear(int V = MAXV) { T.clear(); ans.clear(); Q = 0; for (int i = 0; i < V; i++) present[i].clear(); }
    void addEdge(int v, int w) {
        if (v > w) swap(v, w);
        present[v][w] = Q; q[Q++] = {1, v, w, INT_MAX - 1};
    }
    void removeEdge(int v, int w) {
        if (v > w) swap(v, w);
        int insTime = present[v][w]; q[Q] = {-1, v, w, insTime}; q[insTime].otherTime = Q++; present[v].erase(w);
    }
    void query() { q[Q] = {0, -1, -1, Q}; Q++; }
    void solve(int V) {
        cnt = V; T.reserve(V + Q);
        for (int i = 0; i < V; i++) T.emplace_back(vdef);
        for (int i = 0; i < Q; i++) T.emplace_back(make_pair(q[i].otherTime, i));
        for (int i = 0; i < Q; i++) {
            if (q[i].type == 1) {
                int v = q[i].v, w = q[i].w, o = q[i].otherTime;
                if (connected(v, w)) {
                    pair<int, int> mn = queryPath(v, w);
                    if (mn.first >= o) continue;
                    cut(q[mn.second].v, V + mn.second); cut(q[mn.second].w, V + mn.second); cnt++;
                }
                link(v, V + i); link(w, V + i); cnt--;
            } else if (q[i].type == -1) {
                int v = q[i].v, w = q[i].w, o = q[i].otherTime;
                if (connected(v, V + o)) { cut(v, V + o); cut(w, V + o); cnt++; }
            } else ans.push_back(cnt);
        }
    }
};
