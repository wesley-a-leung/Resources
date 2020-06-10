#pragma once
#include <bits/stdc++.h>
using namespace std;

// Persistent Array
// Indices are 0-indexed
// In practice, has a large constant
// Copy assignment/constructor creates a new version of the data structure
// Time Complexity:
//   constructor, get, set: O(log N)
//   size: O(1)
// Memory Complexity: O(N + Q log N) for Q operations
// Tested:
//   https://dmoj.ca/problem/wac4p5
//   https://dmoj.ca/problem/ioi12p3
template <class T> struct PersistentArray {
  struct Node; using ptr = shared_ptr<Node>;
  struct Node {
    int k; T v; ptr l, r;
    Node(int k, const T &v, const ptr &l = ptr(), const ptr &r = ptr())
        : k(k), v(v), l(l), r(r) {}
  };
  int N; ptr root;
  ptr build(int l, int r, const T &v) {
    if (l >= r) return l == r ? make_shared<Node>(l, v) : ptr();
    int m = l + (r - l) / 2;
    return make_shared<Node>(m, v, build(l, m - 1, v), build(m + 1, r, v));
  };
  ptr dfs(const ptr &x, int k, const T &v) {
    if (k < x->k) return make_shared<Node>(x->k, x->v, dfs(x->l, k, v), x->r);
    else if (k > x->k)
      return make_shared<Node>(x->k, x->v, x->l, dfs(x->r, k, v));
    else return make_shared<Node>(x->k, v, x->l, x->r);
  };
  PersistentArray(int N, const T &v = T()) : N(N), root(build(0, N - 1, v)) {}
  const T &get(int k) const {
    for (ptr x = root; ; x = k < x->k ? x->l : x->r) if (k == x->k)
      return x->v;
  }
  void set(int k, const T &v) { root = dfs(root, k, v); }
  int size() const { return N; }
};
