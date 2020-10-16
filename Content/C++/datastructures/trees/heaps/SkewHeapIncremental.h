#pragma once
#include <bits/stdc++.h>
#if __cplusplus < 201402L
#include "../../../utils/MakeUnique.h"
#endif
using namespace std;

// Skew Heap supporting merges and increments
// Template Arguments:
//   T: the type of each element
//   Cmp: the comparator to compare two values of type T,
//       convention is same as priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
//   Delta: the type of the incremental element
// Functions:
//   empty(): returns whether the heap is empty
//   size(): returns the number of elements in the heap
//   top(): returns the largest element in the heap based on the comparator
//   pop(): pops the largest element in the heap based on the comparator,
//     and returns that element
//   push(key): pushes val onto the heap
//   increment(delta): increments all elements in the heap by delta
//   merge(h): merges the heap h into this heap, destroys heap h
// In practice, has a moderate constant
// Time Complexity:
//   constructor, empty, size, top, increment: O(1)
//   pop, push: O(log N) amortized
//   merge: O(log (N + M)) amortized
// Memory Complexity: O(N)
// Tested:
//   https://codeforces.com/contest/1041/problem/D
template <class T, class Cmp = less<T>, class Delta = T>
struct SkewHeapIncremental {
  Cmp cmp; Delta ddef;
  struct Node {
    T val; Delta delta; unique_ptr<Node> l, r;
    Node(const T &v, const Delta &d) : val(v), delta(d) {}
  };
  int cnt; unique_ptr<Node> root;
  void propagate(unique_ptr<Node> &a) {
    a->val += a->delta;
    if (a->l) a->l->delta += a->delta;
    if (a->r) a->r->delta += a->delta;
    a->delta = ddef;
  }
  unique_ptr<Node> merge(unique_ptr<Node> a, unique_ptr<Node> b) {
    if (!a || !b) return a ? move(a) : move(b);
    propagate(a); propagate(b); if (cmp(a->val, b->val)) a.swap(b);
    a->l.swap(a->r); a->r = merge(move(b), move(a->r)); return move(a);
  }
  SkewHeapIncremental(const Delta &ddef = Delta()) : ddef(ddef), cnt(0) {}
  bool empty() const { return !root; }
  int size() const { return cnt; }
  T top() { propagate(root); return root->val; }
  T pop() {
    propagate(root); T ret = root->val;
    root = merge(move(root->l), move(root->r)); cnt--; return ret;
  }
  void push(const T &val) {
    root = merge(move(root), make_unique<Node>(val, ddef)); cnt++;
  }
  void increment(const Delta &delta) { if (root) root->delta += delta; }
  void merge(SkewHeapIncremental &h) {
    root = merge(move(root), move(h.root)); cnt += h.cnt;
  }
};
