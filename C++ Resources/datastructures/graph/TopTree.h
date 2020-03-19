#pragma once
#include <bits/stdc++.h>
using namespace std;

// Top Tree supporting path and subtree, updates and queries
// Time Complexity:
//   constructor: O(N)
//   makeRoot, findRoot, findParent, cutParent, link: O(log N)
//   updatePath, updateSubtree, queryPath, querySubtree: O(log N)
// Memory Complexity: O(N)

const char NONE = 0, INC = 1, ASSIGN = 2; using Value = int; const Value vdef = 0;
struct Data {
    Value mn, mx, sum; int size; Data() : mn(numeric_limits<Value>::max()), mx(numeric_limits<Value>::lowest()), sum(0), size(0) {}
    Data(const Value &v) : mn(v), mx(v), sum(v), size(1) {}
    Data(const Value &mn, const Value &mx, const Value &sum, int size) : mn(mn), mx(mx), sum(sum), size(size) {}
};
struct Lazy {
    Value lz; char fl; Lazy() : lz(vdef), fl(NONE) {}
    Lazy(const Value &lz, char fl) : lz(lz), fl(fl) {}
};
const Data qdef = Data(); const Lazy ldef = Lazy();
Value applyVal(const Value &v, const Lazy &lz) {
    if (lz.fl == ASSIGN) return lz.lz;
    else if (lz.fl == INC) return v + lz.lz;
    else return v;
}
Data merge(const Data &l, const Data &r) { return Data(min(l.mn, r.mn), max(l.mx, r.mx), l.sum + r.sum, l.size + r.size); }
Lazy mergeLazy(const Lazy &l, const Lazy &r) { return Lazy(applyVal(l.lz, r), max(l.fl, r.fl)); }
Data applyLazy(const Data &l, const Lazy &r) {
    return l.size == 0 ? l : Data(applyVal(l.mn, r), applyVal(l.mx, r), applyVal(l.sum, Lazy(r.lz * l.size, r.fl)), l.size);
}
struct Node {
    int x, p; array<int, 4> ch; Value val; Data path, sbtr, ftr; Lazy lzpath, lzsbtr; bool rev, inr; Node() {}
    Node(int x, const Value &v, bool inr) : x(x), p(0), val(v), lzpath(ldef), lzsbtr(ldef), rev(false), inr(inr) {
        ch.fill(0); path = sbtr = ftr = inr ? qdef : Data(v);
    }
    bool isRoot(bool t); void reverse(); void applyPath(const Lazy &v); void applySbtr(const Lazy &v, bool f); void update(); void propagate();
    int findInd(); void rotate(int t); void splay(int t); void add(int y); void rem();
};
vector<Node> T; vector<int> garbage;
int makeNode(const Value &val, bool inr) {
    if (garbage.empty()) { T.emplace_back(int(T.size()), val, inr); return int(T.size()) - 1; }
    int x = garbage.back(); garbage.pop_back(); T[x] = Node(x, val, inr); return x;
}
void delNode(int x) { garbage.push_back(x); }
bool Node::isRoot(bool t) {
    if (t) return !p || !inr || !T[p].inr;
    else return !p || (x != T[p].ch[0] && x != T[p].ch[1]);
}
void Node::reverse() { rev = !rev; swap(ch[0], ch[1]); }
void Node::applyPath(const Lazy &v) { 
    val = applyVal(val, v); path = applyLazy(path, v); lzpath = mergeLazy(lzpath, v); sbtr = merge(path, ftr);
}
void Node::applySbtr(const Lazy &v, bool f) {
    sbtr = applyLazy(sbtr, v); ftr = applyLazy(ftr, v); lzsbtr = mergeLazy(lzsbtr, v);
    if (f) applyPath(v);
}
void Node::update() {
    if (!x) return;
    path = sbtr = ftr = qdef;
    if (!inr) path = sbtr = Data(val);
    for (int i = 0; i < 2; i++) if (ch[i]) { path = merge(path, T[ch[i]].path); sbtr = merge(sbtr, T[ch[i]].sbtr); ftr = merge(ftr, T[ch[i]].ftr); }
    for (int i = 2; i < 4; i++) if (ch[i]) { sbtr = merge(sbtr, T[ch[i]].sbtr); ftr = merge(ftr, T[ch[i]].sbtr); }
}
void Node::propagate() {
    if (!x) return;
    if (rev) {
        rev = false;
        for (int i = 0; i < 2; i++) if (ch[i]) T[ch[i]].reverse();
    }
    for (int i = 0; i < 4; i++) if (ch[i]) T[ch[i]].applySbtr(lzsbtr, i >= 2);
    for (int i = 0; i < 2; i++) if (ch[i]) T[ch[i]].applyPath(lzpath);
    lzpath = lzsbtr = ldef;
}
int Node::findInd() {
    for (int i = 0; i < 4; i++) if (T[p].ch[i] == x) return i;
    assert(false); return -1;
}
void connect(int ch, int par, int i) {
    if (ch) T[ch].p = par;
    T[par].ch[i] = ch;
}
void Node::rotate(int t) {
    int p = this->p, g = T[p].p; bool isL = x == T[p].ch[t];
    if (g) connect(x, g, T[p].findInd());
    else this->p = 0;
    connect(ch[t ^ isL], p, t ^ !isL); connect(p, x, t ^ isL); T[p].update();
}
void Node::splay(int t) {
    while (!isRoot(t)) {
        int p = this->p, g = T[p].p;
        if (!T[p].isRoot(t)) ((x == T[p].ch[t]) == (p == T[g].ch[t]) ? T[p] : T[x]).rotate(t);
        rotate(t);
    }
    update();
}
void Node::add(int y) {
    T[y].propagate();
    for (int i = 2; i < 4; i++) if (!T[y].ch[i]) { connect(x, y, i); return; }
    int z = makeNode(vdef, true), w = 0;
    for (w = y; T[T[w].ch[2]].inr; w = T[w].ch[2]) T[T[w].ch[2]].propagate();
    connect(T[w].ch[2], z, 2); connect(x, z, 3); connect(z, w, 2); T[z].splay(2);
}
void Node::rem() {
    if (T[p].inr) { connect(T[p].ch[findInd() ^ 1], T[p].p, T[p].findInd()); delNode(p); T[T[p].p].splay(2); }
    else connect(0, p, findInd());
    p = 0;
}
void expose(int x) {
    stack<int, vector<int>> stk; int y = x, z = x;
    for (; z; z = T[z].p) stk.push(z);
    for (; !stk.empty(); stk.pop()) T[stk.top()].propagate();
    T[x].splay(0);
    if (T[x].ch[1]) { z = T[x].ch[1]; T[x].ch[1] = 0; T[z].add(x); T[x].update(); }
    for (; T[x].p; x = z) {
        for (z = T[x].p; T[z].inr; z = T[z].p);
        T[z].splay(0);
        if (T[z].ch[1]) { connect(T[z].ch[1], T[x].p, T[x].findInd()); T[T[x].p].splay(2); }
        else T[x].rem();
        connect(x, z, 1); T[z].update();
    }
    T[y].splay(0);
}
// one-indexed
struct TopTree {
    TopTree(int N) {
        T.clear(); garbage.clear(); T.reserve(2 * N); garbage.reserve(2 * N); makeNode(vdef, false);
        for (int i = 1; i <= N; i++) makeNode(vdef, false);
    }
    template <class It> TopTree(It st, It en) {
        T.clear(); garbage.clear(); T.reserve(2 * (en - st)); garbage.reserve(2 * (en - st)); makeNode(vdef, false);
        for (It i = st; i < en; i++) makeNode(*i, false);
    }
    void makeRoot(int x) { expose(x); T[x].reverse(); }
    int findRoot(int x) { for (; T[x].p; x = T[x].p); return x; }
    int findParent(int x) {
        expose(x); T[T[x].ch[0]].propagate();
        for (x = T[x].ch[0]; x && T[x].ch[1]; x = T[x].ch[1]) T[T[x].ch[1]].propagate();
        return x;
    }
    int cutParent(int x) {
        int y = findParent(x);
        if (y) { expose(y); T[x].rem(); T[y].update(); }
        return y;
    }
    void link(int ch, int par) { // makes par the parent of ch, unless par is in the subtree of ch
        int p = cutParent(ch);
        if (findRoot(ch) != findRoot(par)) p = par;
        if (p) { expose(p); T[ch].add(p); T[p].update(); }
    }
    void updateSubtree(int x, const Lazy &lz) {
        expose(x); T[x].val = applyVal(T[x].val, lz);
        for (int i = 2; i < 4; i++) if (T[x].ch[i]) T[T[x].ch[i]].applySbtr(lz, true);
        T[x].update();
    }
    void updatePath(int x, int y, const Lazy &lz) { makeRoot(x); expose(y); T[x].splay(0); T[x].applyPath(lz); }
    Data querySubtree(int x) {
        expose(x); Data ret(T[x].val);
        for (int i = 2; i < 4; i++) if (T[x].ch[i]) ret = merge(ret, T[T[x].ch[i]].sbtr);
        return ret;
    }
    Data queryPath(int x, int y) { makeRoot(x); expose(y); T[x].splay(0); return T[x].path; }
};
