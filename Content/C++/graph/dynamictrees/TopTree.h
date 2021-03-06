#pragma once
#include <bits/stdc++.h>
using namespace std;

// Top Tree supporting path and subtree operations on a dynamic tree
// Vertices are 0-indexed, with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   C: struct to combine data and lazy values
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       static applyLazy(l, r): returns the value r of type Lazy applied to
//         l of type Data, must be associative
//       static getSegmentVal(v, k): returns the lazy value v when applied over
//         a segment of length k
//       static mergeLazy(l, r): returns the values l of type Lazy merged with
//         r of type Lazy, must be associative
//       static revData(v): reverses the value v of type Data
//     Sample Struct: supporting range assignments and range sum queries
//       struct C {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         static Lazy ldef() { return numeric_limits<int>::min(); }
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         static Data applyLazy(const Data &l, const Lazy &r) { return r; }
//         static Lazy getSegmentVal(const Lazy &v, int k) { return v * k; }
//         static Lazy mergeLazy(const Lazy &l, const Lazy &r) { return r; }
//         static void revData(Data &v) {}
//       };
// Constructor Arguments:
//   N: the number of nodes in the link cut tree
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Functions:
//   makeRoot(x): makes x the root of its connected component
//   lca(x, y): returns the lowest common ancestor of x and y in
//     the current forest, returns -1 if not connected
//   connected(x, y): returns whether x and y are connected
//   link(par, ch): makes par the parent of the node ch, assumes par and ch
//     are not connected, reroots the tree at node par
//   safeLink(par, ch): makes par the parent of the node ch, returns false if
//     already connected, true otherwise, reroots the tree at node par
//   cutParent(x): cuts the edge between node x and its parent, returns false
//     if no parent exists (x is a root), true otherwise
//   findParent(x): returns the parent of node x, -1 if it doesn't exist
//   findRoot(x): returns the root of the forest containing node x
//   depth(x): returns the depth of node x, where the depth of the root is 0
//   kthParent(x): returns the kth parent of node x, where the 0th parent is
//     x, -1 if it doesn't exist
//   updateVertex(x, v): updates the node x with the lazy value v
//   updatePathFromRoot(to, v): updates the path from the root of the forest
//     containing node to, to node to, with the lazy value v
//   updatePath(from, to, v): updates the path from node
//     from to node to, reroots the forest at node from, with the lazy value v,
//     reroots the forest at node from
//   updateSubtree(x): updates the subtree of node x with the lazy value v
//   queryVertex(x): returns the value of node x
//   queryPathFromRoot(to): returns the aggregate value of the path from
//     the root of the forest containing node to, to node to
//   queryPath(from, to): returns the aggregate value of the path
//     from node from to node to, reroots the forest at node from, reroots the
//     forest at node from
//   querySubtree(x): returns the aggregate value of the subtree of node x
// In practice, has a large constant
// Time Complexity:
//   constructor: O(N)
//   makeRoot, lca, connected, link, safeLink, cutParent,
//     findParent, findRoot, depth, kthParent, updateVertex,
//     updatePathFromRoot, updatePath, updateSubtree, queryVertex,
//     queryPathFromRoot, queryPath, querySubtree: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://www.spoj.com/problems/QTREE2/
//   https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite
//   https://judge.yosupo.jp/problem/dynamic_tree_subtree_add_subtree_sum
//   https://dmoj.ca/problem/ds5
template <class C> struct TopTree {
  using Data = typename C::Data; using Lazy = typename C::Lazy;
  struct Node {
    bool rev, aux; int szpath, szvtr; array<Node *, 4> ch; Node *p;
    Lazy lzpath, lzvtr; Data val, path, vtr;
    Node(bool aux, const Data &v)
        : rev(false), aux(aux), szpath(aux ? 0 : 1), szvtr(0),
          p(nullptr), lzpath(C::ldef()), lzvtr(C::ldef()), val(v),
          path(aux ? C::qdef() : v), vtr(C::qdef()) {
      ch.fill(nullptr);
    }
    void applyVal(const Lazy &v) { val = C::applyLazy(val, v); }
    void applyPath(const Lazy &v) {
      applyVal(v); lzpath = C::mergeLazy(lzpath, v);
      if (szpath > 0) path = C::applyLazy(path, C::getSegmentVal(v, szpath));
    }
    void applyVtr(const Lazy &v, bool ap = true) {
      lzvtr = C::mergeLazy(lzvtr, v);
      if (szvtr > 0) vtr = C::applyLazy(vtr, C::getSegmentVal(v, szvtr));
      if (!aux && ap) applyPath(v);
    }
    void update() {
      szpath = aux ? 0 : 1; path = aux ? C::qdef() : val;
      szvtr = 0; vtr = C::qdef(); for (int i = 0; i < 4; i++) if (ch[i]) {
        vtr = C::merge(vtr, ch[i]->vtr); szvtr += ch[i]->szvtr; if (i < 2) {
          path = i ? C::merge(path, ch[i]->path) : C::merge(ch[i]->path, path);
          szpath += ch[i]->szpath;
        } else { vtr = C::merge(vtr, ch[i]->path); szvtr += ch[i]->szpath; }
      }
    }
    void propagate() {
      if (rev) {
        for (int i = 0; i < 2; i++) if (ch[i]) ch[i]->reverse();
        rev = false;
      }
      if (lzpath != C::ldef() && !aux) {
        for (int i = 0; i < 2; i++) if (ch[i]) ch[i]->applyPath(lzpath);
        lzpath = C::ldef();
      }
      if (lzvtr != C::ldef()) {
        for (int i = 0; i < 4; i++) if (ch[i]) ch[i]->applyVtr(lzvtr, i >= 2);
        lzvtr = C::ldef();
      }
    }
    void reverse() { rev = !rev; swap(ch[0], ch[1]); C::revData(path); }
  };
  vector<Node> TR; vector<Node *> deleted, stk;
  Node *makeNode(bool aux, const Data &v) {
    if (deleted.empty()) { TR.emplace_back(aux, v); return &TR.back(); }
    Node *x = deleted.back(); deleted.pop_back(); *x = Node(aux, v); return x;
  }
  int vert(Node *x) { return x - TR.data(); }
  bool isRoot(Node *x, bool t) {
    if (t) return !x->p || !x->aux || !x->p->aux;
    else return !x->p || (x != x->p->ch[0] && x != x->p->ch[1]);
  }
  void connect(Node *x, Node *p, int i) {
    if (x) x->p = p;
    if (i != -1) p->ch[i] = x;
  }
  int findInd(Node *x) {
    for (int i = 0; i < 4; i++) if (x->p->ch[i] == x) return i;
    return -1;
  }
  void rotate(Node *x, int t) {
    Node *p = x->p, *g = p->p; bool isL = x == p->ch[t];
    if (g) connect(x, g, findInd(p));
    else x->p = nullptr;
    connect(x->ch[t ^ isL], p, t ^ !isL); connect(p, x, t ^ isL);
    p->update();
  }
  void splay(Node *x, int t) {
    while (!isRoot(x, t)) {
      Node *p = x->p, *g = p->p;
      if (!isRoot(p, t)) rotate((x == p->ch[t]) == (p == g->ch[t]) ? p : x, t);
      rotate(x, t);
    }
    x->update();
  }
  Node *select(Node *&root, int k) {
    Node *last = nullptr; while (root) {
      (last = root)->propagate();
      int t = root->ch[0] ? root->ch[0]->szpath : 0;
      if (t > k) root = root->ch[0];
      else if (t < k) { root = root->ch[1]; k -= t + 1; }
      else break;
    }
    if (last) splay(root = last, 0);
    return root;
  }
  void add(Node *x, Node *y) {
    Node *z = makeNode(true, C::qdef());
    connect(y->ch[2], z, 2); connect(x, z, 3); connect(z, y, 2); z->update();
  }
  void rem(Node *x) {
    Node *p = x->p, *g = p->p; connect(p->ch[findInd(x) ^ 1], g, findInd(p));
    splay(g, 2); deleted.push_back(x->p); x->p = nullptr;
  }
  void touch(Node *x) {
    for (Node *y = x; y->p; y = y->p) stk.push_back(y->p);
    for (; !stk.empty(); stk.pop_back()) stk.back()->propagate();
    x->propagate();
  }
  Node *nextChain(Node *x) {
    splay(x, 0); if (!x->p) return nullptr;
    Node *p = x->p; splay(p, 2); return p->p;
  }
  Node *access(Node *x) {
    touch(x); Node *last = nullptr; for (Node *y = x; y; y = nextChain(y)) {
      splay(y, 0); if (last) rem(last);
      if (y->ch[1]) add(y->ch[1], y);
      connect(last, y, 1); last = y;
    }
    splay(x, 0); return last;
  }
  void makeRoot(Node *x) { access(x); x->reverse(); }
  Node *findMin(Node *x) {
    for (x->propagate(); x->ch[0]; (x = x->ch[0])->propagate());
    splay(x, 0); return x;
  }
  Node *findMax(Node *x) {
    for (x->propagate(); x->ch[1]; (x = x->ch[1])->propagate());
    splay(x, 0); return x;
  }
  void makeRoot(int x) { makeRoot(&TR[x]); }
  int lca(int x, int y) {
    if (x == y) return x;
    access(&TR[x]); Node *ny = access(&TR[y]); return TR[x].p ? vert(ny) : -1;
  }
  bool connected(int x, int y) { return lca(x, y) != -1; }
  void link(int par, int ch) {
    makeRoot(par); access(&TR[ch]); add(&TR[par], &TR[ch]);
  }
  bool safeLink(int par, int ch) {
    if (connected(par, ch)) return false;
    link(par, ch); return true;
  }
  bool cutParent(int x) {
    access(&TR[x]); if (!TR[x].ch[0]) return false;
    TR[x].ch[0]->p = nullptr; TR[x].ch[0] = nullptr; return true;
  }
  int findParent(int x) {
    access(&TR[x]); return TR[x].ch[0] ? vert(findMax(TR[x].ch[0])) : -1;
  }
  int findRoot(int x) { access(&TR[x]); return vert(findMin(&TR[x])); }
  int depth(int x) {
    access(&TR[x]); return TR[x].ch[0] ? TR[x].ch[0]->szpath : 0;
  }
  int kthParent(int x, int k) {
    int d = depth(x); Node *nx = &TR[x];
    return k <= d ? vert(select(nx, d - k)) : -1;
  }
  void updateVertex(int x, const Lazy &v) {
    access(&TR[x]); TR[x].applyVal(v); TR[x].update();
  }
  void updatePathFromRoot(int to, const Lazy &v) {
    access(&TR[to]); TR[to].applyPath(v);
  }
  bool updatePath(int from, int to, const Lazy &v) {
    makeRoot(from); access(&TR[to]);
    if (from != to && !TR[from].p) return false;
    TR[to].applyPath(v); return true;
  }
  void updateSubtree(int x, const Lazy &v) {
    access(&TR[x]); TR[x].applyVal(v);
    for (int i = 2; i < 4; i++) if (TR[x].ch[i]) TR[x].ch[i]->applyVtr(v);
  }
  Data queryVertex(int x) { access(&TR[x]); return TR[x].val; }
  Data queryPathFromRoot(int to) { access(&TR[to]); return TR[to].sbtr; }
  Data queryPath(int from, int to) {
    makeRoot(from); access(&TR[to]);
    return from == to || TR[from].p ? TR[to].path : C::qdef();
  }
  Data querySubtree(int x) {
    access(&TR[x]); Data ret = TR[x].val;
    for (int i = 2; i < 4; i++) if (TR[x].ch[i]) {
      ret = C::merge(C::merge(ret, TR[x].ch[i]->vtr), TR[x].ch[i]->path);
    }
    return ret;
  }
  template <class F> TopTree(int N, F f) {
    TR.reserve(N * 2); for (int i = 0; i < N; i++) makeNode(false, f());
  }
  template <class It>
  TopTree(It st, It en) : TopTree(en - st, [&] { *st++; }) {}
};
