#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/trees/binarysearchtrees/Splay.h"
using namespace std;

// Link Cut Tree backed by a splay tree
// Vertices are 0-indexed, constructor with iterators is an exclusive range
// Accepts a generic node class as a template parameter (such as the
//   structs in BSTNode.h)
// Node: struct with at least following
//     (more may be required by the underlying Tree being used):
//   Data: typedef/using of the value type this node stores
//   Lazy: typedef/using of the lazy type used to update the node
//     (even if no lazy propagation occurs)
//   sz: integer representing the size of the subtree
//   RANGE_UPDATES: const static bool indicating whether range updates
//     are supported
//   RANGE_QUERIES: const static bool indicating whether range queries
//     are supported
//   RANGE_REVERSALS: const static bool indicating whether range reversals
//     are supported
//   HAS_PAR: const static bool indicating whether this node has
//     a parent pointer, which MUST be true for a link cut tree
//   l: pointer of the same node type to the left child
//   r: pointer of the same node type to the right child
//   p: pointer of the same node type to the parent
//   val: Data representing the value of the node
//   update: void() that updates the size of subtree,
//     along with any additional information
//   propagate: void() that pushes information lazily to the children
//   apply: void(const Lazy &v) that updates the information of the node
//     (including optional aggregate and lazy information)
// If RANGE_UPDATES is true, then the following are required to support
//     path updates:
//   lz: Lazy representing the lazy information that will be pushed to
//     the child nodes
// If RANGE_QUERIES is true, then the following are required to support
//     path queries:
//   sbtr: Data representing the aggregate data of the subtree
//   qdef: static Data() returning the query default value
// If RANGE_REVERSALS is true, then the following are required to support
//     unrooted link cut trees:
//   reverse: void() that reverses the subtree rooted at that node
//     (aggregate data and any lazy flags should be reversed)
// Time Complexity:
//   constructor: O(N)
//   makeRoot, lca, connected, link, safeLink, linkParent, cut, cutParent,
//     findParent, findRoot, depth, kthParent, updateVertex,
//     updatePathFromRoot, updatePath, queryVertex, queryPathFromRoot,
//     queryPath: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/coi08p2
//   https://dmoj.ca/problem/ccoprep16c2q3
//   https://dmoj.ca/problem/ds5easy
//   https://www.spoj.com/problems/QTREE2/
//   https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite
//   https://oj.uz/problem/view/JOI13_synchronization
template <class Node> struct LinkCutTree : public Splay<Node, vector<Node>> {
  using Tree = Splay<Node, vector<Node>>;
  using Tree::TR; using Tree::makeNode; using Tree::splay; using Tree::select;
  using Data = typename Node::Data; using Lazy = typename Node::Lazy;
  int vert(Node *x) { return x - TR.data(); }
  Node *access(Node *x) {
    Node *last = nullptr;
    for (Node *y = x; y; y = y->p) { splay(y); y->r = last; last = y; }
    splay(x); return last;
  }
  template <const int _ = Node::RANGE_REVERSALS>
  typename enable_if<_>::type makeRoot(Node *x) { access(x); x->reverse(); }
  Node *findMin(Node *x) {
    for (x->propagate(); x->l; (x = x->l)->propagate());
    splay(x); return x;
  }
  Node *findMax(Node *x) {
    for (x->propagate(); x->r; (x = x->r)->propagate());
    splay(x); return x;
  }
  template <const int _ = Node::RANGE_REVERSALS>
  typename enable_if<_>::type makeRoot(int x) { makeRoot(&TR[x]); }
  int lca(int x, int y) {
    if (x == y) return x;
    access(&TR[x]); Node *ny = access(&TR[y]); return TR[x].p ? vert(ny) : -1;
  }
  bool connected(int x, int y) { return lca(x, y) != -1; }
  template <const int _ = Node::RANGE_REVERSALS>
  typename enable_if<_>::type link(int x, int y) {
    makeRoot(y); TR[y].p = &TR[x];
  }
  template <const int _ = Node::RANGE_REVERSALS>
  typename enable_if<_, bool>::type safeLink(int x, int y) {
    if (connected(x, y)) return false;
    link(x, y); return true;
  }
  bool linkParent(int par, int ch) {
    access(&TR[ch]); if (TR[ch].l) return false;
    TR[ch].p = &TR[par]; return true;
  }
  template <const int _ = Node::RANGE_REVERSALS>
  typename enable_if<_, bool>::type cut(int x, int y) {
    makeRoot(x); access(&TR[y]);
    if (&TR[x] != TR[y].l || TR[x].r) return false;
    TR[y].l->p = nullptr; TR[y].l = nullptr; return true;
  }
  bool cutParent(int x) {
    access(&TR[x]); if (!TR[x].l) return false;
    TR[x].l->p = nullptr; TR[x].l = nullptr; return true;
  }
  int findParent(int x) {
    access(&TR[x]); return TR[x].l ? vert(findMax(TR[x].l)) : -1;
  }
  int findRoot(int x) { access(&TR[x]); return vert(findMin(&TR[x])); }
  int depth(int x) { access(&TR[x]); return TR[x].l ? TR[x].l->sz : 0; }
  int kthParent(int x, int k) {
    int d = depth(x); Node *nx = &TR[x];
    return k <= d ? vert(select(nx, d - k)) : -1;
  }
  void updateVertex(int x, const Lazy &v) {
    access(&TR[x]); Node *l = TR[x].l; TR[x].l = nullptr;
    TR[x].apply(v); TR[x].propagate(); TR[x].update(); TR[x].l = l;
  }
  template <const int _ = Node::RANGE_UPDATES>
  typename enable_if<_>::type updatePathFromRoot(int to, const Lazy &v) {
    access(&TR[to]); TR[to].apply(v);
  }
  template <const int _ = Node::RANGE_UPDATES && Node::RANGE_REVERSALS>
  typename enable_if<_, bool>::type updatePath(
      int from, int to, const Lazy &v) {
    makeRoot(from); access(&TR[to]);
    if (from != to && !TR[from].p) return false;
    TR[to].apply(v); return true;
  }
  Data queryVertex(int x) { access(&TR[x]); return TR[x].val; }
  template <const int _ = Node::RANGE_QUERIES>
  typename enable_if<_, Data>::type queryPathFromRoot(int to) {
    access(&TR[to]); return TR[to].sbtr;
  }
  template <const int _ = Node::RANGE_QUERIES && Node::RANGE_REVERSALS>
  typename enable_if<_, Data>::type queryPath(int from, int to) {
    makeRoot(from); access(&TR[to]);
    return from == to || TR[from].p ? TR[to].sbtr : Node::qdef();
  }
  template <class F> LinkCutTree(int N, F f) {
    TR.reserve(N); for (int i = 0; i < N; i++) makeNode(f());
  }
  template <class It> LinkCutTree(It st, It en)
      : LinkCutTree(en - st, [&] { *st++; }) {}
};
