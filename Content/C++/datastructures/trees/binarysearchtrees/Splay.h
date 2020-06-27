#pragma once
#include <bits/stdc++.h>
using namespace std;

// Generic Splay Tree node operations supporting a generic node class (such
//   as the structs in BSTNode.h)
// Indices are 0-indexed and ranges are inclusive
// In practice, has a moderate constant, not as fast as segment trees
//   and slightly faster than Treap
// makeNode creates a new node passing v to the node constructor
// applyToRange applies the function f to a node x where x is the disconnected
//   subtree with indices in the range [i, j] for the tree rooted at root
// select finds the kth node in the tree rooted at root,
//   and makes it the new root (or the last node accessed if null)
// index finds the index of node x in the tree rooted at root,
//   and makes it the new root
// getFirst returns the first node y (and its index) in the tree rooted at root
//   where cmp(y->val, v) returns false,
// and makes it the new root (or the last node accessed if null)
// build builds a treap with N nodes using a generating function f
//   that passes a single argument to the node constructor
// clear adds all nodes in the subtree of x to the deleted buffer
// Node must have the following:
//   constructor: takes a single argument with the information for the node
//   HAS_PAR: const static bool indicating whether this node has
//     a parent pointer, which MUST be true for splay
//   sz: integer representing the size of the subtree
//   l: pointer of the same node type to the left child
//   r: pointer of the same node type to the right child
//   p: pointer of the same node type to the parent
//   update: void() that updates the size of subtree,
//     along with any additional information
//   propagate: void() that pushes information lazily to the children
//   If getFirst is called, then the following is also required:
//     val: any type representing the value of the node
// Can be used in conjunction with DynamicRangeOperations to support
//   range operations using splay node operations
// Time Complexity:
//   build, clear: O(N)
//   makeNode: O(1)
//   splay, applyToRange, index, select, getFirst: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds4 (applyToRange, select, getFirst)
//   https://codeforces.com/contest/1288/problem/E (applyToRange, index)
//   https://codeforces.com/contest/863/problem/D (applyToRange)
//   https://dmoj.ca/problem/dmpg17g2 (applyToRange)
//   https://dmoj.ca/problem/acc1p1 (applyToRange)
//   https://dmoj.ca/problem/noi05p2 (applyToRange)
template <class _Node, class Container = deque<_Node>> struct Splay {
  using Node = _Node; Container TR; deque<Node*> deleted;
  static_assert(Node::HAS_PAR, "Splay Node must have parent pointer");
  template <class T> Node *makeNode(const T &v) {
    if (deleted.empty()) { TR.emplace_back(v); return &TR.back(); }
    Node *x = deleted.back(); deleted.pop_back();
    *x = typename Container::value_type(v); return x;
  }
  bool isRoot(Node *x) { return !x->p || (x != x->p->l && x != x->p->r); }
  void connect(Node *x, Node *p, bool hasCh, bool isL) {
    if (x) x->p = p;
    if (hasCh) (isL ? p->l : p->r) = x;
  }
  void rotate(Node *x) {
    Node *p = x->p, *g = p->p; bool isRootP = isRoot(p), isL = x == p->l;
    connect(isL ? x->r : x->l, p, true, isL); connect(p, x, true, !isL);
    connect(x, g, !isRootP, !isRootP && p == g->l); p->update();
  }
  void splay(Node *x) {
    while (!isRoot(x)) {
      Node *p = x->p, *g = p->p; if (!isRoot(p)) g->propagate();
      p->propagate(); x->propagate();
      if (!isRoot(p)) rotate((x == p->l) == (p == g->l) ? p : x);
      rotate(x);
    }
    x->propagate(); x->update();
  }
  template <class F> void applyToRange(Node *&root, int i, int j, F f) {
    int sz = root ? root->sz : 0;
    if (i <= 0 && sz - 1 <= j) {
      f(root); if (root) { root->propagate(); root->update(); }
    } else if (i <= 0) {
      Node *l = select(root, j + 1)->l; connect(nullptr, root, true, true);
      root->update(); connect(l, nullptr, false, true); f(l);
      if (l) { l->propagate(); l->update(); }
      connect(l, root, true, true); root->update();
    } else if (sz - 1 <= j) {
      Node *r = select(root, i - 1)->r; connect(nullptr, root, true, false);
      root->update(); connect(r, nullptr, false, false); f(r);
      if (r) { r->propagate(); r->update(); }
      connect(r, root, true, false); root->update();
    } else {
      Node *r = select(root, i - 1)->r; connect(nullptr, root, true, false);
      root->update(); connect(r, nullptr, false, false);
      Node *l = select(r, j - i + 1)->l; connect(nullptr, r, true, true);
      r->update(); connect(l, nullptr, false, true); f(l);
      if (l) { l->propagate(); l->update(); }
      connect(l, r, true, true); r->update(); connect(r, root, true, false);
      root->update();
    }
  }
  Node *select(Node *&root, int k) {
    Node *last = nullptr; while (root) {
      (last = root)->propagate(); int t = root->l ? root->l->sz : 0;
      if (t > k) root = root->l;
      else if (t < k) { root = root->r; k -= t + 1; }
      else break;
    }
    if (root) splay(root);
    else if (last) splay(root = last);
    return root;
  }
  int index(Node *&root, Node *x) {
    root = x; if (!root) return -1;
    splay(root); return root->l ? root->l->sz : 0;
  }
  template <class T, class Comp>
  pair<int, Node *> getFirst(Node *&root, const T &v, Comp cmp) {
    pair<int, Node *> ret(0, nullptr); Node *last = nullptr;
    for (Node *x = root; x;) {
      (last = x)->propagate();
      if (!cmp(x->val, v)) { root = ret.second = x; x = x->l; }
      else { ret.first += 1 + (x->l ? x->l->sz : 0); x = x->r; }
    }
    if (root) splay(root);
    else if (last) splay(root = last);
    return ret;
  }
  template <class F> Node *buildRec(int l, int r, F &f) {
    if (l > r) return nullptr;
    int m = l + (r - l) / 2; Node *left = buildRec(l, m - 1, f);
    Node *ret = makeNode(f()), *right = buildRec(m + 1, r, f);
    connect(left, ret, ret, true); connect(right, ret, ret, false);
    ret->update(); return ret;
  }
  template <class F> Node *build(int N, F f) { return buildRec(0, N - 1, f); }
  void clear(Node *x) {
    if (!x) return;
    clear(x->l); deleted.push_back(x); clear(x->r);
  }
};
