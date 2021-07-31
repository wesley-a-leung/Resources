#pragma once
#include <bits/stdc++.h>
using namespace std;

// Weighted Union Find by size with path compression
// Indices are 0-indexed
// Template Arguments:
//   T: the type of the weight
//   Op: a struct with the cumulative operation to combine weights
//     Required Functions:
//       operator (l, r): combines the values l and r
// Constructor Arguments:
//   W: a vector of type T of the weights of each element in the set
//   op: an instance of the Op struct
// Fields:
//   UF: a vector of integers representing the parent of each element in the
//     tree, or the negative of the size of the set if that element is a root
//   W: a vector of type T representing the weight of all elements in the
//     connected component if and only if that element is a root
//   cnt: the current number of disjoint sets
// Functions:
//   find(v): finds the root of the set containing element v
//   join(v, w): merges the sets containing v and w, returns true if they
//     were originally disjoint before the merge, false otherwise
//   connected(v, w): returns true if v and w are in the same set,
//     false otherwise
//   getSize(v): returns the size of the set containing element v
//   getWeight(v): returns the weight of the set containing element v
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   find, join, connected: O(alpha N) amortized, O(log N) worse case
//   getSize, getWeight: O(alpha N) amortized, O(log N) worse case
// Memory Complexity: O(N)
// Tested:
//   https://dmoj.ca/problem/noi15p5
template <class T, class Op> struct WeightedUnionFind {
  vector<int> UF; vector<T> W; int cnt; Op op;
  WeightedUnionFind(const vector<T> &W, Op op = Op())
      : UF(W.size(), -1), W(W), cnt(W.size()), op(op) {}
  int find(int v) { return UF[v] < 0 ? v : UF[v] = find(UF[v]); }
  bool join(int v, int w) {
    if ((v = find(v)) == (w = find(w))) return false;
    if (UF[v] > UF[w]) swap(v, w);
    UF[v] += UF[w]; UF[w] = v; W[v] = op(W[v], W[w]); cnt--; return true;
  }
  bool connected(int v, int w) { return find(v) == find(w); }
  int getSize(int v) { return -UF[find(v)]; }
  T getWeight(int v) { return W[find(v)]; }
};
