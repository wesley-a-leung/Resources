#pragma once
#include <bits/stdc++.h>
#include "../../../utils/Random.h"
using namespace std;

// Generic Treap node operations supporting a generic node class (such
//   as the structs in BSTNode)
// Can be used in conjunction with DynamicRangeOperations to support
//   range operations using treap node operations
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   _Node: a generic node class (sample structs are in BSTNode)
//     Required Fields:
//       static const HAS_PAR: a boolean indicating whether a
//         parent pointer exists
//       sz: the number of nodes in the subtree
//       l: a pointer to the left child
//       r: a pointer to the right child
//       p: only required if index is called, a pointer to the parent
//       val: only required if getFirst is called, the value being stored
//     Required Functions:
//       constructor(v): initializes a node with the value v
//       update(): updates the current node's information based on its children
//       propagate(): propagates the current node's lazy information (including
//         rev) to its children
// Fields:
//   Node: equivalent to _Node
//   TR: a deque of all nodes (including potentially deleted nodes)
//   deleted: a deque of node pointers to store deleted nodes
// Functions:
//   merge(x, l, r): merges the nodes l and r into the node x
//   split(x, l, r, lsz): splits the node x into the nodes l and r with
//     the size of l being lsz
//   makeNode(v): creates a new node passing v to the node constructor
//   applyToRange(root, i, j, f): applies the function f (accepting a
//     node pointer or reference to a node pointer) to a node x where x
//     is the disconnected subtree with indices in the range [i, j] for the
//     tree rooted at root (passed by reference)
//   select(x, k): returns the kth node in the subtree of x
//   index(root, x): only valid if Node::HAS_PAR is true, returns the index
//     of node x in its tree (root is a dummy argument to maintain
//     consistency with Splay API)
//   getFirst(x, v, cmp): returns the first node y (and its index) in the
//     subtree of x where cmp(y->val, v) returns false
//   build(N, f): builds a treap with N nodes using a generating function f
//     and returns the ith element on the ith call, which is passes
//     to the node constructor
//   clear(x): adds all nodes in the subtree of x to the deleted buffer
// In practice, has a moderate constant, not as fast as segment trees
//   and slightly slower than Splay
// Time Complexity:
//   build: O(N) expected
//   clear: O(N)
//   makeNode: O(1)
//   merge, split, applyToRange, select, index, getFirst: O(log N) expected
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
template <class _Node> struct Treap {
  using Node = _Node;
  struct TreapNode : public Node {
    long long pri;
    template <class T> TreapNode(const T &v)
        : Node(v), pri(uniform_int_distribution<long long>()(rng64)) {}
  };
  deque<TreapNode> TR; deque<Node *> deleted;
  template <class T> Node *makeNode(const T &v) {
    if (deleted.empty()) { TR.emplace_back(v); return &TR.back(); }
    Node *x = deleted.back(); deleted.pop_back(); *x = TreapNode(v); return x;
  }
  long long pri(Node *x) { return static_cast<TreapNode *>(x)->pri; }
  void merge(Node *&x, Node *l, Node *r) {
    if (l) l->propagate();
    if (r) r->propagate();
    if (!l || !r) { x = l ? l : r; return; }
    if (pri(l) > pri(r)) { merge(l->r, l->r, r); x = l; setP(x->r, x); }
    else { merge(r->l, l, r->l); x = r; setP(x->l, x); }
    x->update();
  }
  template <const int _ = Node::HAS_PAR>
  typename enable_if<_>::type setP(Node *x, Node *p) { if (x) x->p = p; }
  template <const int _ = Node::HAS_PAR>
  typename enable_if<!_>::type setP(Node *, Node *) {}
  void split(Node *x, Node *&l, Node *&r, int lsz) {
    if (!x) { l = r = nullptr; return; }
    x->propagate(); setP(x, nullptr); int t = x->l ? x->l->sz : 0;
    if (lsz <= t) { split(x->l, l, x->l, lsz); r = x; setP(x->l, x); }
    else { split(x->r, x->r, r, lsz - t - 1); l = x; setP(x->r, x); }
    x->update();
  }
  template <class F> void applyToRange(Node *&root, int i, int j, F f) {
    Node *l, *m, *r; split(root, l, m, i); split(m, m, r, j - i + 1); f(m);
    merge(root, l, m); merge(root, root, r);
  }
  Node *select(Node *x, int k) {
    while (x) {
      x->propagate(); int t = x->l ? x->l->sz : 0;
      if (t > k) x = x->l;
      else if (t < k) { x = x->r; k -= t + 1; }
      else break;
    }
    return x;
  }
  template <const int _ = Node::HAS_PAR>
  typename enable_if<_, int>::type index(Node *root, Node *x) {
    function<int(Node *, Node *)> dfs = [&] (Node *x, Node *ch) {
      if (!x) return ch ? 0 : -1;
      int ind = dfs(x->p, x); x->propagate();
      if (!ch) return ind + (x->l ? x->l->sz : 0);
      else if (x->l == ch) return ind;
      return ind + 1 + (x->l ? x->l->sz : 0);
    };
    return dfs(x, nullptr);
  }
  template <class T, class Comp>
  pair<int, Node *> getFirst(Node *x, const T &v, Comp cmp) {
    pair<int, Node *> ret(0, nullptr); while (x) {
      x->propagate();
      if (!cmp(x->val, v)) { ret.second = x; x = x->l; }
      else { ret.first += 1 + (x->l ? x->l->sz : 0); x = x->r; }
    }
    return ret;
  }
  template <class F> Node *buildRec(int l, int r, F &f) {
    if (l > r) return nullptr;
    if (l == r) return makeNode(f());
    int m = l + (r - l) / 2; Node *ret;
    Node *left = buildRec(l, m, f), *right = buildRec(m + 1, r, f);
    merge(ret, left, right); return ret;
  }
  template <class F> Node *build(int N, F f) { return buildRec(0, N - 1, f); }
  void clear(Node *x) {
    if (!x) return;
    clear(x->l); deleted.push_back(x); clear(x->r);
  }
};
