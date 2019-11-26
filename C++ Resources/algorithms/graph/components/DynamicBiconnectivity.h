#pragma once
#include <bits/stdc++.h>
using namespace std;

// Support queries for the number of bridges in a graph, after edges have been added or removed
// Time Complexity: O(V + Q * log(V + Q))
// Memory Complexity: O(V + Q)

const int NO_COVER = INT_MIN, NO_DEL = INT_MAX;
void check(int &a, const int &b) { if ((a > b && b != NO_COVER) || a == NO_COVER) a = b; }
// Stripped down version of Link Cut Tree
struct Node {
    Node *l, *r, *p; bool rev, isEdge; int edgeCnt, coveredCntSub, coverLazy, covered, coveredSub, del, delMin;
    Node(bool isEdge) : l(nullptr), r(nullptr), p(nullptr), rev(false), isEdge(isEdge), edgeCnt(isEdge), coveredCntSub(0),
        coverLazy(NO_COVER), covered(NO_COVER), coveredSub(NO_COVER), del(NO_DEL), delMin(NO_DEL) {}
    bool isRoot(); int getCoveredCnt(); void update(); void propagate(); void removeCover(int cover);
    void rotate(); void splay(); Node *expose(); void makeRoot(); void setDel(int tim);
};
bool Node::isRoot() { return !p || (this != p->l && this != p->r); }
int Node::getCoveredCnt() { return coverLazy == NO_COVER ? coveredCntSub : edgeCnt; }
void Node::update() {
    edgeCnt = isEdge; coveredCntSub = isEdge && (covered != NO_COVER); coveredSub = covered; delMin = del;
    if (l) {
        check(coveredSub, l->coveredSub); check(coveredSub, l->coverLazy);
        edgeCnt += l->edgeCnt; coveredCntSub += l->getCoveredCnt(); delMin = min(delMin, l->delMin);
    }
    if (r) {
        check(coveredSub, r->coveredSub); check(coveredSub, r->coverLazy);
        edgeCnt += r->edgeCnt; coveredCntSub += r->getCoveredCnt(); delMin = min(delMin, r->delMin);
    }
}
void Node::propagate() {
    if (rev) {
        swap(l, r); rev = false;
        if (l) l->rev = !l->rev;
        if (r) r->rev = !r->rev;
    }
    if (coverLazy != NO_COVER) {
        covered = max(covered, coverLazy); check(coveredSub, coverLazy); coveredCntSub = edgeCnt;
        if (l) l->coverLazy = max(l->coverLazy, coverLazy);
        if (r) r->coverLazy = max(r->coverLazy, coverLazy);
        coverLazy = NO_COVER;
    }
}
void Node::removeCover(int cover) {
    if (coverLazy <= cover) coverLazy = NO_COVER;
    if (coveredSub == NO_COVER || coveredSub > cover) return;
    if (covered <= cover) covered = NO_COVER;
    if (l) l->removeCover(cover);
    if (r) r->removeCover(cover);
    propagate(); update();
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
void Node::setDel(int tim) { del = tim; propagate(); update(); expose(); }
template <const int MAXV, const int MAXQ> struct DynamicBiconnectivity {
    int V, Q = 0, bridges; bool isTreeEdge[MAXQ]; vector<Node> T; vector<int> ans; unordered_map<int, int> present[MAXV];
    struct Query { int type, v, w, otherTime; } q[MAXQ];
    bool connected(int x, int y) {
        if (x == y) return true;
        T[x].expose(); T[y].expose(); return T[x].p;
    }
    void link(int par, int ch) { T[ch].makeRoot(); T[ch].p = &T[par]; }
    void cut(int x, int y) { T[x].makeRoot(); T[y].expose(); T[y].r->p = nullptr; T[y].r = nullptr; }
    void cover(int x, int y, int coverId) {
        T[y].makeRoot(); T[x].expose(); bridges += T[x].getCoveredCnt(); T[x].coverLazy = coverId; bridges -= T[x].getCoveredCnt();
    }
    void unCover(int x, int y, int coverId) {
        T[y].makeRoot(); T[x].expose(); bridges += T[x].getCoveredCnt(); T[x].removeCover(coverId); bridges -= T[x].getCoveredCnt();
    }
    int pathMinDel(int x, int y) { T[y].makeRoot(); T[x].expose(); return T[x].delMin; }
    void insertTreeEdge(int id) {
        T[V + id].setDel(q[id].otherTime); link(V + id, q[id].v); link(V + id, q[id].w); bridges++; isTreeEdge[id] = true;
    }
    void removeTreeEdge(int id) {
        cut(V + id, q[id].v); cut(V + id, q[id].w); bridges += T[V + id].getCoveredCnt() - 1; isTreeEdge[id] = false;
    }
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
        this->V = V; bridges = 0; T.reserve(V + Q); fill(isTreeEdge, isTreeEdge + Q, false);
        for (int i = 0; i < V; i++) T.emplace_back(false);
        for (int i = 0; i < Q; i++) T.emplace_back(true);
        for (int i = 0; i < Q; i++) {
            int v = q[i].v, w = q[i].w, o = q[i].otherTime;
            if (q[i].type == 1) {
                if (connected(v, w)) {
                    int minCover = pathMinDel(v, w), coverTime = i;
                    if (minCover < o) { coverTime = q[minCover].otherTime; removeTreeEdge(coverTime); insertTreeEdge(i); }
                    cover(q[coverTime].v, q[coverTime].w, q[coverTime].otherTime);
                } else insertTreeEdge(i);
            } else if (q[i].type == -1) {
                if (isTreeEdge[q[i].otherTime]) removeTreeEdge(q[i].otherTime);
                else unCover(q[i].v, q[i].w, i);
            } else ans.push_back(bridges);
        }
    }
};
