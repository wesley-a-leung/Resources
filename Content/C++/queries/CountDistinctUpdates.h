#pragma once
#include <bits/stdc++.h>
#include "../datastructures/trees/fenwicktrees/SparseFenwickTrees2D.h"
using namespace std;

// Supports online queries for the number of distinct elements in the
//   range [l, r] for an array A of length N with point updates
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   C: a container representing a mapping from type T to a set of integers
// Constructor Arguments:
//   A: a vector of type T of the initial values in the array
//   SCALE: the value to scale sqrt by
//   ...args: arguments to pass to the constructor of an instance of type C
// Functions:
//   update(i, v): updates index i with the value v
//   query(l, r): returns the number of distinct values in the range [l, r]
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(N log N)
//   update: O(log N)
//   query: O(log N sqrt Q)
// Memory Complexity: O(N + Q log N) for Q updates
// Tested:
//   https://dmoj.ca/problem/mnyc17p6
template <class T, class C = map<T, set<int>>> struct CountDistinctUpdates {
  vector<T> A; C M; SemiSparseFenwickTree2DSimple<int> ft;
  void add(int i, const T &v) {
    set<int> &mv = M[v]; auto it = mv.upper_bound(++i);
    int j = it == mv.begin() ? 0 : *prev(it);
    ft.add(j, i); if (it != mv.end()) { ft.rem(j, *it); ft.add(i, *it); }
    mv.insert(it, i);
  }
  void rem(int i, const T &v) {
    set<int> &mv = M[v]; auto it = mv.erase(mv.find(++i));
    int j = it == mv.begin() ? 0 : *prev(it);
    ft.rem(j, i); if (it != mv.end()) { ft.rem(i, *it); ft.add(j, *it); }
  }
  void update(int i, const T &v) { rem(i, A[i]); add(i, A[i] = v); }
  int query(int l, int r) { return ft.query(l, l + 1, r + 1); }
  template <class ...Args>
  CountDistinctUpdates(const vector<T> &A, double SCALE = 1, Args &&...args)
      : A(A), M(forward<Args>(args)...), ft(A.size() + 1, SCALE) {
    for (int i = 0; i < int(A.size()); i++) add(i, A[i]);
  }
};
