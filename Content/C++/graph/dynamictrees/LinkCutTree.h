#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/trees/binarysearchtrees/Splay.h"
using namespace std;

// Link Cut Tree supporting path operations on a dynamic tree,
//   backed by a splay tree
// Vertices are 0-indexed, with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   Node: a generic node class (sample structs are in BSTNode)
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//       static const RANGE_UPDATES: a boolean indicating whether
//         range updates are permitted
//       static const RANGE_QUERIES: a boolean indicating whether
//         range queries are permitted
//       static const RANGE_REVERSALS: a boolean indicating whether
//         range reversals are permitted
//       sz: the number of nodes in the subtree
//       l: a pointer to the left child
//       r: a pointer to the right child
//       p: a pointer to the parent
//       val: only required if getFirst is called, the value being stored
//       sbtr: only required if RANGE_QUERIES is true to support path queries,
//         the aggregate value of type Data for the subtree
//     Required Functions:
//       constructor(v): initializes a node with the value v
//       update(): updates the current node's information based on its children
//       propagate(): propagates the current node's lazy information (including
//         rev) to its children
//       apply(v): applies the lazy value v to the node
//       reverse(): only required if RANGE_REVERSALS is true to support
//         rerooting, reverse this node's subtree (aggregate data
//         and any lazy flags should be reversed)
//       static qdef(): returns the query default value
// Constructor Arguments:
//   N: the number of nodes in the link cut tree
//   f: a generating function that returns the ith element on the ith call,
//     which is passed to the node constructor
//   st: an iterator pointing to the first element in the array,
//     whos elements are passed to the node constructor
//   en: an iterator pointing to after the last element in the array,
//     whos elements are passed to the node constructor
// Functions:
//   makeRoot(x): only valid if Node::RANGE_REVERSALS is true, makes x the
//     root of its connected component
//   lca(x, y): returns the lowest common ancestor of x and y in
//     the current forest, returns -1 if not connected
//   connected(x, y): returns whether x and y are connected
//   link(x, y): only valid if Node::RANGE_REVERSALS is true, links the nodes
//     x and y, assumes x and y are not connected, reroots the tree at node y
//   safeLink(x, y): only valid if Node::RANGE_REVERSALS is true, links
//     the nodes x and y, returns false if already connected, true otherwise,
//     reroots the tree at node y
//   linkParent(par, ch): makes par the parent of the node ch, returns false if
//     par and ch are already connected, true otherwise
//   cut(x, y): only valid if Node::RANGE_REVERSALS is true, cuts the edge
//     between nodes x and y, returns false if this edge doesn't exist,
//     true otherwise, reroots the forest at node x
//   cutParent(x): cuts the edge between node x and its parent, returns false
//     if no parent exists (x is a root), true otherwise
//   findParent(x): returns the parent of node x, -1 if it doesn't exist
//   findRoot(x): returns the root of the forest containing node x
//   depth(x): returns the depth of node x, where the depth of the root is 0
//   kthParent(x): returns the kth parent of node x, where the 0th parent is
//     x, -1 if it doesn't exist
//   updateVertex(x, v): updates the node x with the lazy value v
//   updatePathFromRoot(to, v): only valid if Node::RANGE_UPDATES is true,
//     updates the path from the root of the forest containing
//     node to, to node to, with the lazy value v
//   updatePath(from, to, v): only valid if Node::RANGE_UPDATES and
//     Node::RANGE_REVERSALS are true, updates the path from node
//     from to node to, reroots the forest at node from, with the lazy value v,
//     reroots the forest at node from
//   queryVertex(x): returns the value of node x
//   queryPathFromRoot(to): only valid if Node::RANGE_QUERIES is true,
//     returns the aggregate value of the path from the root of the forest
//     containing node to, to node to
//   queryPath(from, to): only valid if Node::RANGE_QUERIES and
//     Node::RANGE_REVERSALS are true, returns the aggregate value of the path
//     from node from to node to, reroots the forest at node from, reroots the
//     forest at node from
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(N)
//   makeRoot, lca, connected, link, safeLink, linkParent, cut, cutParent,
//     findParent, findRoot, depth, kthParent, updateVertex,
//     updatePathFromRoot, updatePath, queryVertex, queryPathFromRoot,
//     queryPath: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/coi08p2
//   https://codeforces.com/contest/13/problem/E
//   https://dmoj.ca/problem/ds5easy
//   https://www.spoj.com/problems/QTREE2/
//   https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite
//   https://oj.uz/problem/view/JOI13_synchronization
template <class Node> struct LCT : public Splay<Node, vector<Node>> {
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
  template <class F> LCT(int N, F f) {
    TR.reserve(N); for (int i = 0; i < N; i++) makeNode(f());
  }
  template <class It> LCT(It st, It en) : LCT(en - st, [&] { *st++; }) {}
};
