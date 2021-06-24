#pragma once
#include <bits/stdc++.h>
using namespace std;

// Persistent Array where copy assignment/constructor creates
//   a new version of the data structure
// Indices are 0-indexed
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   v: the default value to fill the array with
// Functions:
//   get(k): returns a constant reference to the kth element in the
//     current version
//   set(k, v): sets the kth element to index v in the current version
//   size(): returns the size of the array
// In practice, has a moderate constant
// Time Complexity:
//   constructor, get, set: O(log N)
//   size: O(1)
// Memory Complexity: O(N + Q log N) for Q operations
// Tested:
//   https://judge.yosupo.jp/problem/persistent_queue
template <class T> struct PersistentArray {
  struct Node; using ptr = shared_ptr<Node>;
  struct Node {
    T v; ptr l, r;
    Node(const T &v, const ptr &l = ptr(), const ptr &r = ptr())
        : v(v), l(l), r(r) {}
  };
  int N; ptr root;
  template <class F> ptr build(int l, int r, const F &f) {
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
