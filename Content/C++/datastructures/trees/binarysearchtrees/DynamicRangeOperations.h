#pragma once
#include <bits/stdc++.h>
#include "Splay.h"
using namespace std;

// Operations on ranges of a dynamic array, backed by a splay tree
// Supports point/range updates/queries, range reversals,
//   and binary searching as long as Node contains the appropriate flags
// Also supports insertion and erasing at an index or with a comparator
//   (as long as all values in the tree are sorted by the same comparator),
//   with insert_at inserting an element or range before the specified index
// Indices are 0-indexed and ranges are inclusive 
// Template Arguments:
//   Node: typedef/using of the node class containing information about
//       each node in the tree
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//       static const RANGE_UPDATES: a boolean indicating whether
//         range updates are permitted
//       static const RANGE_QUERIES: a boolean indicating whether
//         range queries are permitted
//       static const RANGE_REVERSALS: a boolean indicating whether
//         range reversals are permitted
//       l: a pointer to the left child
//       r: a pointer to the right child
//       val: the value of type Data being stored
//       lz: only required if RANGE_UPDATES is true, the value of type
//         Lazy to be propagated
//       sbtr: only required if RANGE_QUERIES is true, the aggregate
//         value of type Data for the subtree
//     Required Functions:
//       constructor(v): initializes a node with the value v
//       propagate(): propagates the current node's lazy information
//         (including rev) to its children
//       apply(v): applies the lazy value v to the node
//       reverse(): only required if RANGE_REVERSALS is true, reverse
//         this node's subtree (aggregate data and any lazy
//         flags should be reversed)
//       static qdef(): only required if RANGE_QUERIES is true, returns the
//         query default value
// Constructor Arguments:
//   N: the size of the array
//   A: the array
// Functions:
//   insert_at(i, v): inserts a node before index i by passing v to the
//     node constructor
//   insert_at(i, A): inserts the elements in the vector A before index i,
//     where the elements are passed to the node constructor in the order
//     they appear
//   insert(v, cmp): inserts a node before the first of node y where
//     cmp(y->val, v) returns false, by passing v to the node constructor
//   erase_at(i): erases the node at index i
//   erase_at(j): erases the nodes between index i and j inclusive
//   erase(v, cmp): erases the first node y where both cmp(y->val, v) and
//     cmp(v, y->val) returns false
//   update(i, v): updates the node at index i with the lazy value v
//   update(i, j, v): only valid if Node::RANGE_UPDATES is true, updates the
//     nodes between index i and j inclusive with the lazy value v
//   reverse(i, j): only valid if Node::RANGE_REVERSALS is true, reverses the
//     nodes between index i and j inclusive
//   size(): returns the number of nodes in the trees
//   at(i): returns the value of the node at index i
//   lower_bound(v, cmp): returns the index and pointer to the value of the
//     first node y such that cmp(y->val, v) returns false, returns
//     {size(), nullptr} if none exist
//   upper_bound(v, cmp): returns the index and pointer to the value of the
//     first node y such that cmp(v, y->val) returns true, returns
//     {size(), nullptr} if none exist
//   find(v, cmp): returns the index and pointer to the value of the
//     first node y such that both cmp(y->val, v) and cmp(v, y->val) return
//     false, returns {size(), nullptr} if none exist
//   query(i, j): only valid if Node::RANGE_QUERIES is true, returns the
//     aggregate value of the nodes between index i and j inclusive,
//     Node::qdef() if empty
//   values(): returns a vector of the values of all nodes in the tree
// Time Complexity:
//   constructor: O(N)
//   insert, insert_at: O(log N + M) amortized for M inserted elements
//   erase: erase_at: O(log N + M) amortized for M deleted elements
//   update, reverse, at, lower_bound, upper_bound, find, query:
//     O(log N) amortized
//   values: O(N)
//   size: O(1)
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/ds4 (insert, erase, at, find, values)
//   https://codeforces.com/contest/863/problem/D (reverse)
//   https://dmoj.ca/problem/acc1p1 (reverse, range queries)
//   https://dmoj.ca/problem/noi05p2
//     (insert_at, erase_at, range update, reverse, range queries)
//   https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum
//     (insert_at, erase_at, range update, reverse, range queries)
template <class Node> struct DynamicRangeOperations : public Splay<Node> {
  using Data = typename Node::Data; using Lazy = typename Node::Lazy;
  Node *root; using Tree = Splay<Node>;
  using Tree::makeNode; using Tree::applyToRange; using Tree::select;
  using Tree::getFirst; using Tree::build; using Tree::clear;
  template <class T>
  DynamicRangeOperations(const vector<T> &A) : root(build(A)) {}
  DynamicRangeOperations() : root(nullptr) {}
  template <class T> void insert_at(int i, const T &v) {
    applyToRange(root, i, i - 1, [&] (Node *&x) { x = makeNode(v); });
  }
  template <class T> void insert_at(int i, const vector<T> &A) {
    applyToRange(root, i, i - 1, [&] (Node *&x) { x = build(A); }); 
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
