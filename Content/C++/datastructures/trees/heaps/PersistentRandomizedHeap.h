#pragma once
#include <bits/stdc++.h>
#include "../../../utils/Random.h"
using namespace std;

// Persistent Heap supporting merges where copy assignment/constructor creates
//   a new version of the data structure
// Template Arguments:
//   T: the type of each element
//   Cmp: the comparator to compare two values of type T,
//       convention is same as std::priority_queue in STL
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Functions:
//   empty(): returns whether the current heap is empty
//   size(): returns the number of elements in the current heap
//   top(): returns the largest element in the current heap based
//     on the comparator
//   pop(): pops the largest element in the current heap based
//     on the comparator, and returns that element
//   push(key): pushes val onto the current heap
//   merge(h): merges the heap h into the current heap
// In practice, has a moderate constant
// Time Complexity:
//   constructor, empty, size, top: O(1)
//   pop, push: O(log N) expected
//   merge: O(log (N + M)) expected where M is the size of heap h
// Memory Complexity: O(N + Q log N) for Q operations
// Tested:
//   https://dmoj.ca/problem/wac4p5
template <class T, class Cmp = less<T>> struct PersistentRandomizedHeap {
  struct Node; using ptr = shared_ptr<Node>;
  struct Node {
    T val; ptr l, r;
    Node(const T &v, const ptr &l = ptr(), const ptr &r = ptr())
        : val(v), l(l), r(r) {}
  };
  Cmp cmp; int cnt; ptr root;
  ptr merge(ptr a, ptr b) {
    if (!a || !b) return a ? a : b;
    if (cmp(a->val, b->val)) a.swap(b);
    return rng() % 2 ? make_shared<Node>(a->val, a->l, merge(a->r, b))
                     : make_shared<Node>(a->val, merge(a->l, b), a->r);
  }
  PersistentRandomizedHeap() : cnt(0) {}
  bool empty() const { return !root; }
  int size() const { return cnt; }
  T top() { return root->val; }
  T pop() {
    T ret = root->val; root = merge(root->l, root->r); cnt--; return ret;
  }
  void push(const T &val) {
    root = merge(root, make_shared<Node>(val)); cnt++;
  }
  void merge(const PersistentRandomizedHeap &h) {
    root = merge(root, h.root); cnt += h.cnt;
  }
};
