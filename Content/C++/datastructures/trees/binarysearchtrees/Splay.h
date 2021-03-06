#pragma once
#include <bits/stdc++.h>
using namespace std;

// Generic Splay Tree node operations supporting a generic node class (such
//   as the structs in BSTNode)
// Can be used in conjunction with DynamicRangeOperations to support
//   range operations using splay node operations
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   _Node: a generic node class (sample structs are in BSTNode)
//     Required Fields:
//       static const HAS_PAR: a boolean indicating whether a
//         parent pointer exists, must be true for Splay
//       sz: the number of nodes in the subtree
//       l: a pointer to the left child
//       r: a pointer to the right child
//       p: a pointer to the parent
//       val: only required if getFirst is called, the value being stored
//     Required Functions:
//       constructor(v): initializes a node with the value v
//       update(): updates the current node's information based on its children
//       propagate(): propagates the current node's lazy information (including
//         rev) to its children
//   Container: a container to contain _Node objects (or some child class of
//     _Node), recommended to use deque<_Node> or vector<_Node> if there is
//    a fixed number of nodes and TR can be reserved beforehand
// Fields:
//   Node: equivalent to _Node
//   TR: a container of type Container to store the nodes (including
//     potentially deleted nodes)
//   deleted: a deque of node pointers to store deleted nodes
// Functions:
//   splay(x): splays the node x to the root of the tree
//   makeNode(v): creates a new node passing v to the node constructor
//   applyToRange(root, i, j, f): applies the function f (accepting a
//     node pointer or reference to a node pointer) to a node x where x
//     is the disconnected subtree with indices in the range [i, j] for the
//     tree rooted at root (passed by reference)
//   select(root, k): returns the kth node in the subtree of root,
//     and makes it the new root (or the last node accessed if null)
//   index(root, x): returns the index of node x in the tree rooted at root,
//     and makes it the new root
//   getFirst(root, v, cmp): returns the first node y (and its index) in the
//     tree rooted at root where cmp(y->val, v) returns false, and makes it the
//     new root (or the last node accessed if null)
//   build(N, f): builds a splay tree with N nodes using a generating
//     function f and returns the ith element on the ith call, which is passes
//     to the node constructor
//   clear(x): adds all nodes in the subtree of x to the deleted buffer
// In practice, has a moderate constant, not as fast as segment trees
// Time Complexity:
//   build, clear: O(N)
//   makeNode: O(1)
//   splay, applyToRange, select, index, getFirst: O(log N) amortized
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds4 (applyToRange, select, getFirst)
//   https://codeforces.com/contest/1288/problem/E (applyToRange, index)
//   https://codeforces.com/contest/863/problem/D (applyToRange)
//   https://dmoj.ca/problem/dmpg17g2 (applyToRange)
//   https://dmoj.ca/problem/acc1p1 (applyToRange)
//   https://dmoj.ca/problem/noi05p2 (applyToRange)
//   https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
//     (applyToRange)
template <class _Node, class Container = deque<_Node>> struct Splay {
  using Node = _Node; Container TR; deque<Node *> deleted;
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
    if (i == 0 && j == (root ? root->sz : 0) - 1) {
      f(root); if (root) { root->propagate(); root->update(); }
    } else {
      Node *t = i ? select(root, i - 1)->r : select(root, j + 1)->l;
      connect(nullptr, root, true, !i); root->update();
      connect(t, nullptr, false, !i); applyToRange(t, 0, j - i, f);
      connect(t, root, true, !i); root->update();
    }
  }
  Node *select(Node *&root, int k) {
    Node *last = nullptr; while (root) {
      (last = root)->propagate(); int t = root->l ? root->l->sz : 0;
      if (t > k) root = root->l;
      else if (t < k) { root = root->r; k -= t + 1; }
      else break;
    }
    if (last) splay(root = last);
    return root;
  }
  int index(Node *&root, Node *x) {
    root = x; if (!root) return -1;
    splay(root); return root->l ? root->l->sz : 0;
  }
  template <class T, class Comp>
  pair<int, Node *> getFirst(Node *&root, const T &v, Comp cmp) {
    pair<int, Node *> ret(0, nullptr); Node *last = nullptr; while (root) {
      (last = root)->propagate();
      if (!cmp(root->val, v)) { ret.second = root; root = root->l; }
      else { ret.first += 1 + (root->l ? root->l->sz : 0); root = root->r; }
    }
    if (last) splay(root = last);
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
