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
    T v; ptr l, r;
    Node(const T &v, const ptr &l = ptr(), const ptr &r = ptr())
        : v(v), l(l), r(r) {}
  };
  int N; ptr root;
  template <class F> ptr build(int l, int r, F &f) {
    if (l >= r) return l == r ? make_shared<Node>(f()) : ptr();
    int m = l + (r - l) / 2;
    ptr left = build(l, m - 1, f); T a = f(); ptr right = build(m + 1, r, f);
    return make_shared<Node>(a, left, right);
  }
  ptr dfs(const ptr &x, int l, int r, int k, const T &v) {
    int m = l + (r - l) / 2;
    if (k < m) return make_shared<Node>(x->v, dfs(x->l, l, m - 1, k, v), x->r);
    else if (k > m)
      return make_shared<Node>(x->v, x->l, dfs(x->r, m + 1, r, k, v));
    else return make_shared<Node>(v, x->l, x->r);
  }
  template <class F> PersistentArray(int N, F f)
      : N(N), root(build(0, N - 1, f)) {}
  template <class It> PersistentArray(It st, It en)
      : PersistentArray(en - st, [&] { return *st++; }) {}
  PersistentArray(int N, const T &v = T())
      : PersistentArray(N, [&] { return v; }) {}
  const T &get(int k) const {
    ptr x = root; for (int l = 0, r = N - 1, m; k != (m = l + (r - l) / 2);) {
      if (k < m) { x = x->l; r = m - 1; }
      else { x = x->r; l = m + 1; }
    }
    return x->v;
  }
  void set(int k, const T &v) { root = dfs(root, 0, N - 1, k, v); }
  int size() const { return N; }
};
