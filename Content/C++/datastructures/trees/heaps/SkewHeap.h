#pragma once
#include <bits/stdc++.h>
#if __cplusplus < 201402L
#include "../../../utils/MakeUnique.h"
#endif
using namespace std;

// Skew Heap supporting merges
// Template Arguments:
//   T: the type of each element
//   Cmp: the comparator to compare two values of type T,
//       convention is same as priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Functions:
//   empty(): returns whether the heap is empty
//   size(): returns the number of elements in the heap
//   top(): returns the largest element in the heap based on the comparator
//   pop(): pops the largest element in the heap based on the comparator
//   push(key): pushes val onto the heap
//   merge(h): merges the heap h into this heap, destroys heap h
// In practice, has a moderate constant
// Time Complexity:
//   constructor, empty, size, top: O(1)
//   pop, push: O(log N) amortized
//   merge: O(log (N + M)) amortized where M is the size of heap h
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/apio16p2
template <class T, class Cmp = less<T>> struct SkewHeap {
  struct Node { T val; unique_ptr<Node> l, r; Node(const T &v) : val(v) {} };
  Cmp cmp; int cnt; unique_ptr<Node> root;
  unique_ptr<Node> merge(unique_ptr<Node> a, unique_ptr<Node> b) {
    if (!a || !b) return a ? move(a) : move(b);
    if (cmp(a->val, b->val)) a.swap(b);
    a->l.swap(a->r); a->r = merge(move(b), move(a->r)); return move(a);
  }
  SkewHeap() : cnt(0) {}
  bool empty() const { return !root; }
  int size() const { return cnt; }
  T top() const { return root->val; }
  T pop() {
    T ret = root->val; root = merge(move(root->l), move(root->r)); cnt--;
    return ret;
  }
  void push(const T &val) {
    root = merge(move(root), make_unique<Node>(val)); cnt++;
  }
  void merge(SkewHeap &h) {
    root = merge(move(root), move(h.root)); cnt += h.cnt;
  }
};
