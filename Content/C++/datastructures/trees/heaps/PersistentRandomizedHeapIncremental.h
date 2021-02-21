#pragma once
#include <bits/stdc++.h>
#include "../../../utils/Random.h"
using namespace std;

// Persistent Heap supporting merges and increments where
//   copy assignment/constructor creates a new version of the data structure
// Template Arguments:
//   T: the type of each element
//   Cmp: the comparator to compare two values of type T,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
//   Delta: the type of the incremental element
// Functions:
//   empty(): returns whether the current heap is empty
//   size(): returns the number of elements in the current heap
//   top(): returns the largest element in the current heap based
//     on the comparator
//   pop(): pops the largest element in the current heap based
//     on the comparator, and returns that element
//   push(key): pushes val onto the current heap
//   increment(delta): increments all elements in the current heap by delta
//   merge(h): merges the heap h into the current heap
// In practice, has a moderate constant
// Time Complexity:
//   constructor, empty, size, top, increment: O(1)
//   pop, push: O(log N) expected
//   merge: O(log (N + M)) expected where M is the size of heap h
// Memory Complexity: O(N + Q log N) for Q operations
// Tested:
//   https://dmoj.ca/problem/wac4p5
template <class T, class Cmp = less<T>, class Delta = T>
struct PersistentRandomizedHeapIncremental {
  struct Node; using ptr = shared_ptr<Node>;
  struct Node {
    T val; Delta delta; ptr l, r;
    Node(const T &v, const Delta &d,
         const ptr &l = ptr(), const ptr &r = ptr())
        : val(v), delta(d), l(l), r(r) {}
  };
  Cmp cmp; Delta ddef; int cnt; ptr root;
  void propagate(ptr &a) {
    a->val += a->delta;
    if (a->l) { a->l = make_shared<Node>(*a->l); a->l->delta += a->delta; }
    if (a->r) { a->r = make_shared<Node>(*a->r); a->r->delta += a->delta; }
    a->delta = ddef;
  }
  ptr merge(ptr a, ptr b) {
    if (!a || !b) return a ? a : b;
    propagate(a); propagate(b); if (cmp(a->val, b->val)) a.swap(b);
    return rng() % 2
           ? make_shared<Node>(a->val, a->delta, a->l, merge(a->r, b))
           : make_shared<Node>(a->val, a->delta, merge(a->l, b), a->r);
  }
  PersistentRandomizedHeapIncremental(const Delta &ddef = Delta())
      : ddef(ddef), cnt(0) {}
  bool empty() const { return !root; }
  int size() const { return cnt; }
  T top() { propagate(root); return root->val; }
  T pop() {
    propagate(root); T ret = root->val;
    root = merge(root->l, root->r); cnt--; return ret;
  }
  void push(const T &val) {
    root = merge(root, make_shared<Node>(val, ddef)); cnt++;
  }
  void increment(const Delta &delta) {
    if (root) { root = make_shared<Node>(*root); root->delta += delta; }
  }
  void merge(const PersistentRandomizedHeapIncremental &h) {
    root = merge(root, h.root); cnt += h.cnt;
  }
};
