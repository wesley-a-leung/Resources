#pragma once
#include <bits/stdc++.h>
using namespace std;

// Operations on ranges of a dynamic array,
//   backed by a generic binary search tree (such as Treap or Splay)
// Supports point/range updates/queries, range reversals,
//   and binary searching as long as Node contains the appropriate flags
// Also supports insertion and erasing at an index or with a comparator
//   (as long as all values in the tree are sorted by the same comparator),
//   with insert_at inserting an element or range before the specified index
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   Tree: either Treap or Splay, each may have additional required fields
//       or functions
//     Required Fields:
//       Node: typedef/using of the node class containing information about
//           each node in the tree
//         Required Fields:
//           Data: typedef/using for the data type
//           Lazy: typedef/using for the lazy type
//           static const RANGE_UPDATES: a boolean indicating whether
//             range updates are permitted
//           static const RANGE_QUERIES: a boolean indicating whether
//             range queries are permitted
//           static const RANGE_REVERSALS: a boolean indicating whether
//             range reversals are permitted
//           l: a pointer to the left child
//           r: a pointer to the right child
//           val: the value of type Data being stored
//           lz: only required if RANGE_UPDATES is true, the value of type
//             Lazy to be propagated
//           sbtr: only required if RANGE_QUERIES is true, the aggregate
//             value of type Data for the subtree
//         Required Functions:
//           constructor(v): initializes a node with the value v
//           propagate(): propagates the current node's lazy information
//             (including rev) to its children
//           apply(v): applies the lazy value v to the node
//           reverse(): only required if RANGE_REVERSALS is true, marks
//             this node's subtree for reversal (aggregate data and any lazy
//             flags should be reversed)
//           static qdef(): only required if RANGE_QUERIES is true, returns the
//             query default value
//     Required Functions:
//       makeNode(v): creates a new node passing v to the node constructor
//       applyToRange(root, i, j, f): applies the function f (accepting a
//         node pointer or reference to a node pointer) to a node x where x
//         is the disconnected subtree with indices in the range [i, j] for the
//         tree rooted at root (passed by reference)
//       select(x, k): returns the kth node in the subtree of x
//       getFirst(x, v, cmp): returns the first node y (and its index) in the
//         subtree of x where cmp(y->val, v) returns false
//       build(N, f): builds a tree with N nodes using a generating function f
//         and returns the ith element on the ith call, which is passes
//         to the node constructor
//       clear(x): adds all nodes in the subtree of x to the deleted buffer
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call,
//     which is passed to the node constructor
//   st: an iterator pointing to the first element in the array,
//     whos elements are passed to the node constructor
//   en: an iterator pointing to after the last element in the array,
//     whos elements are passed to the node constructor
// Functions:
//   insert_at(i, v): inserts a node before index i by passing v to the
//     node constructor
//   insert_at(i, n, f): inserts n nodes before index i by passing the return
//     value of the kth call to f to the node constructor for each of the
//     n calls
//   insert_at(i, st, en): inserts en - st nodes before index i where st is an
//     iterator pointing to before the first element in the array, and en is an
//     iterator pointing to after the last element in the array, where the
//     elements are passed to the node constructor
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
