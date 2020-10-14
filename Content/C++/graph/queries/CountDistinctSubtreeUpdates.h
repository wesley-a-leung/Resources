#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/trees/fenwicktrees/FenwickTree1D.h"
#include "../lowestcommonancestor/LowestCommonAncestor.h"
#include "SubtreeQueries.h"
using namespace std;

// Supports online queries for the number of distinct elements in a
//   subtree for a forest G with V vertices with vertex updates
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of each element
//   C: a container representing a mapping from type T to a set of integers
// Constructor Arguments:
//   G: a generic forest data structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
//   A: a vector of type T of the intial values of each vertex
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
//   ...args: arguments to pass to the constructor of an instance of type C
// Functions:
//   update(v, val): update vertex v with the value val
//   query(v): returns the number of distinct elements in the subtree of
//     vertex v
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V log V)
//   update, query: O(log V)
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/dmopc20c1p5
template <class T, class C = map<T, set<int>>>
struct CountDistinctSubtreeUpdates {
  struct R {
    using Data = int; using Lazy = int; FenwickTree1D<Data> FT;
    static Data qdef() { return 0; }
    R(const vector<Data> &A) : FT(A.begin(), A.end()) {}
    void update(int l, int, const Lazy &val) { FT.update(l, val); }
    Data query(int l, int r) { return FT.query(l, r); }
  };
  vector<T> A; C M; LCA<> lca; SubtreeQueries<R, false> sbtr;
  void upd(int v, const T &val, int delta) {
    int u = -1, w = -1; set<int> &mv = M[val];
    auto it = delta == 1 ? mv.upper_bound(sbtr.pre[v])
                         : mv.erase(mv.find(sbtr.pre[v]));
    if (it != mv.begin())
      sbtr.updateVertex(lca.lca(u = sbtr.vert[*prev(it)], v), -delta);
    if (it != mv.end())
      sbtr.updateVertex(lca.lca(v, w = sbtr.vert[*it]), -delta);
    if (u != -1 && w != -1) sbtr.updateVertex(lca.lca(u, w), delta);
    sbtr.updateVertex(v, delta); if (delta == 1) mv.insert(it, sbtr.pre[v]);
  }
  void update(int v, const T &val) { upd(v, A[v], -1); upd(v, A[v] = val, 1); }
  int query(int v) { return sbtr.querySubtree(v); }
  template <class Forest, class ...Args> CountDistinctSubtreeUpdates(
      const Forest &G, const vector<T> &A,
      const vector<int> &roots = vector<int>(), Args &&...args)
      : A(A), M(forward<Args>(args)...),
        lca(G, roots), sbtr(G, vector<int>(A.size(), 0), roots) {
    for (int v = 0; v < int(G.size()); v++) upd(v, A[v], 1);
  }
  template <class Forest, class ...Args>
  CountDistinctSubtreeUpdates(const Forest &G, const vector<T> &A, int rt,
                              Args &&...args)
      : CountDistinctSubtreeUpdates(G, A, vector<int>{rt},
                                    forward<Args>(args)...) {}
};
