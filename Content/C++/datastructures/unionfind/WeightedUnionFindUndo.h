#pragma once
#include <bits/stdc++.h>
using namespace std;

// Weighted Union Find by size supporting undos
// Indices are 0-indexed
// Template Arguments:
//   T: the type of the weight
//   Op: a struct with the cumulative operation to combine weights
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutative
// Constructor Arguments:
//   W: a vector of type T of the weights of each element in the set
//   op: an instance of the Op struct
// Fields:
//   UF: a vector of integers representing the parent of each element in the
//     tree, or the negative of the size of the set if that element is a root
//   W: a vector of type T representing the weight of all elements in the
//     connected component if and only if that element is a root
//   cnt: the current number of disjoint sets
//   history: a vector of tuples storing the history of all join calls
// Functions:
//   find(v): finds the root of the set containing element v
//   join(v, w): merges the sets containing v and w, returns true if they
//     were originally disjoint before the merge, false otherwise
//   undo(): undoes the last call to join regardless of its return value
//     by popping from the history stack
//   connected(v, w): returns true if v and w are in the same set,
//     false otherwise
//   getSize(v): returns the size of the set containing element v
//   getWeight(v): returns the weight of the set containing element v
// In practice, has a small constant
// Time Complexity:
//   constructor: O(N)
//   find, join, connected, getSize, getWeight: O(log N)
//   undo: O(1)
// Memory Complexity: O(N + Q) for Q calls to join
// Tested:
//   https://judge.yosupo.jp/problem/dynamic_graph_vertex_add_component_sum
template <class T, class Op> struct WeightedUnionFindUndo {
  vector<int> UF; vector<T> W; int cnt;
  vector<tuple<int, int, int, T, T>> history; Op op;
  WeightedUnionFindUndo(vector<T> W, Op op = Op())
      : UF(W.size(), -1), W(move(W)), cnt(UF.size()), op(op) {
    history.reserve(W.size());
  }
  int find(int v) { return UF[v] < 0 ? v : find(UF[v]); }
  bool join(int v, int w) {
    if ((v = find(v)) == (w = find(w))) {
      history.emplace_back(v, w, 0, W[v], W[w]); return false;
    }
    if (UF[v] > UF[w]) swap(v, w);
    history.emplace_back(v, w, UF[w], W[v], W[w]);
    UF[v] += UF[w]; UF[w] = v; W[v] = op(move(W[v]), move(W[w])); cnt--;
    return true;
  }
  void undo() {
    int v, w, ufw; T wv, ww; tie(v, w, ufw, wv, ww) = history.back();
    history.pop_back(); if (ufw == 0) return;
    UF[w] = ufw; UF[v] -= UF[w]; W[v] = wv; W[w] = ww; cnt++;
  }
  bool connected(int v, int w) { return find(v) == find(w); }
  int getSize(int v) { return -UF[find(v)]; }
  T &getWeight(int v) { return W[find(v)]; }
};
