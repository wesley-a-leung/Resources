#pragma once
#include <bits/stdc++.h>
using namespace std;

// Operations on ranges of a dynamic array,
//   backed by a generic binary search tree (such as Splay.h or Treap.h)
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Supports point/range updates/queries, range reversals,
//   and binary searching as long as Node contains the appropriate flags
// Also supports insertion and erasing at an index or with a comparator
//   (as long as all values in the tree are sorted by the same comparator),
//   with insert_at inserting an element or range before the specified index
// cmp for insert must be of type bool(const Data &, const T &)
// Tree should be of type Treap or Splay, or any struct that has the following:
//   Node: struct with at least following
//       (more may be required by the underlying Tree being used):
//     Data: typedef/using of the value type this node stores
//     Lazy: typedef/using of the lazy type used to update the node
//       (even if no lazy propagation occurs)
//     sz: integer representing the size of the subtree
//     RANGE_UPDATES: const static bool indicating whether range updates
//       are supported
//     RANGE_QUERIES: const static bool indicating whether range queries
//       are supported
//     RANGE_REVERSALS: const static bool indicating whether range reversals
//       are supported
//     l: pointer of the same node type to the left child
//     r: pointer of the same node type to the right child
//     val: Data representing the value of the node
//     propagate: void() that pushes information lazily to the children
//     apply: void(const Lazy &v) that updates the information of the node
//       (including optional aggregate and lazy information)
//   If RANGE_UPDATES is true, then the following are required:
//     lz: Lazy representing the lazy information that will be pushed to
//       the child nodes
//   If RANGE_QUERIES is true, then the following are required:
//     sbtr: Data representing the aggregate data of the subtree
//     qdef: static Data() returning the query default value
//   If RANGE_REVERSALS is true, then the following are required:
//     reverse: void() that reverses the subtree rooted at that node
//       (aggregate data and any lazy flags should be reversed)
//   makeNode: Node *(const T &) that returns a new node with
//     the argument passed to the constructor
//   applyToRange: void(Node *&, int, int, void(Node *&)) that applies
//     a function to a node pointer to the disconnected subtree
//     of the given range
//   select: Node *(Node *&, int) that selects the kth node in a tree
//   getFirst: pair<int, Node *>(Node *&, const T &,
//                               bool(const Data &, const T &))
//     that finds the first node and its index
//     where cmp(x->val, val) returns false
//   build: Node *(int, T()) that returns a node representing
//     the tree of a specified size with a function that returns
//     the argument passed to the constructor
//   clear: void(Node *) that clears/erases a subtree's nodes
// Time Complexity if Treap or Splay is used:
//   constructor: O(N) expected for Treap,
//                O(N) for Splay
//   insert, insert_at: O(log N + M) expected for Treap,
//                      O(log N + M) amortized for Splay,
//                      each for M inserted elements
//   erase: erase_at: O(log N + M) expected for Treap,
//                    O(log N + M) amortized for Splay,
//                    each for M deleted elements
//   update, reverse, at, lower_bound, upper_bound, find, query:
//     O(log N) expected for Treap,
//     O(log N) amortized for Splay
//   values: O(N)
//   size: O(1)
// Memory Complexity if Treap or Splay is used: O(N)
// Tested:
//   https://dmoj.ca/problem/ds4 (insert, erase, at, find, values)
//   https://codeforces.com/contest/863/problem/D (reverse)
//   https://dmoj.ca/problem/dmpg17g2 (point update, range queries)
//   https://dmoj.ca/problem/acc1p1 (reverse, range queries)
//   https://dmoj.ca/problem/noi05p2
//     (insert_at, erase_at, range update, reverse, range queries)
template <class Tree> struct DynamicRangeOperations : public Tree {
  using Node = typename Tree::Node; using Data = typename Node::Data;
  using Lazy = typename Node::Lazy; Node *root;
  using Tree::makeNode; using Tree::applyToRange; using Tree::select;
  using Tree::getFirst; using Tree::build; using Tree::clear;
  template <class F> DynamicRangeOperations(int N, F f) : root(build(N, f)) {}
  template <class It> DynamicRangeOperations(It st, It en)
      : DynamicRangeOperations(en - st, [&] { return *st++; }) {}
  DynamicRangeOperations() : root(nullptr) {}
  template <class T> void insert_at(int i, const T &v) {
    applyToRange(root, i, i - 1, [&] (Node *&x) { x = makeNode(v); });
  }
  template <class F> void insert_at(int i, int n, F f) {
    applyToRange(root, i, i - 1, [&] (Node *&x) { x = build(n, f); }); 
  }
  template <class It> void insert_at(int i, It st, It en) {
    insert_at(i, en - st, [&] { return *st++; });
  }
  template <class T, class Comp> void insert(const T &v, Comp cmp) {
    insert_at(getFirst(root, v, cmp).first, v);
  }
  void erase_at(int i) {
    applyToRange(root, i, i, [&] (Node *&x) { clear(x); x = nullptr; });
  }
  void erase_at(int i, int j) {
    if (i <= j)
      applyToRange(root, i, j, [&] (Node *&x) { clear(x); x = nullptr; });
  }
  template <class Comp> void erase(const Data &v, Comp cmp) {
    pair<int, Node *> p = getFirst(root, v, cmp);
    if (p.second && !cmp(p.second->val, v) && !cmp(v, p.second->val))
      erase_at(p.first);
  }
  void update(int i, const Lazy &v) {
    applyToRange(root, i, i, [&] (Node *&x) { x->apply(v); });
  }
  template <const int _ = Node::RANGE_UPDATES>
  typename enable_if<_>::type update(int i, int j, const Lazy &v) {
    if (i <= j) applyToRange(root, i, j, [&] (Node *&x) { x->apply(v); });
  }
  template <const int _ = Node::RANGE_REVERSALS>
  typename enable_if<_>::type reverse(int i, int j) {
    if (i <= j) applyToRange(root, i, j, [&] (Node *&x) { x->reverse(); });
  }
  int size() { return root ? root->sz : 0; }
  Data at(int i) { return select(root, i)->val; }
  template <class Comp>
  pair<int, Data *> lower_bound(const Data &v, Comp cmp) {
    pair<int, Node *> p = getFirst(root, v, cmp);
    return make_pair(p.first, p.second ? &p.second->val : nullptr);
  }
  template <class Comp>
  pair<int, Data *> upper_bound(const Data &v, Comp cmp) {
    return lower_bound(v, [&] (const Data &a, const Data &b) {
      return !cmp(b, a);
    });
  }
  template <class Comp> pair<int, Data *> find(const Data &v, Comp cmp) {
    pair<int, Data *> ret = lower_bound(v, cmp);
    if (!ret.second || cmp(v, *(ret.second)) || cmp(*(ret.second), v))
      return make_pair(size(), nullptr);
    return ret;
  }
  template <const int _ = Node::RANGE_QUERIES>
  typename enable_if<_, Data>::type query(int i, int j) {
    Data ret = Node::qdef();
    if (i <= j) applyToRange(root, i, j, [&] (Node *&x) { ret = x->sbtr; });
    return ret;
  }
  vector<Data> values() {
    vector<Data> ret; ret.reserve(size());
    function<void(Node *)> dfs = [&] (Node *x) {
      if (!x) return;
      x->propagate(); dfs(x->l); ret.push_back(x->val); dfs(x->r);
    };
    dfs(root); return ret;
  }
};
