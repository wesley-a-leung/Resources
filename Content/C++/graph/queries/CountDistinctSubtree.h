#pragma once
#include <bits/stdc++.h>
#include "../lowestcommonancestor/LowestCommonAncestor.h"
#include "SubtreeQueries.h"
using namespace std;

// Supports online queries for the number of distinct elements in a
//   subtree for a static forest G with V vertices
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   G: a generic forest data structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
//   A: a vector of type T of the values of each vertex
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
// Functions:
//   query(v): returns the number of distinct elements in the subtree of
//     vertex v
// In practice, has a moderate constant, slightly faster than SmallToLargeTree
// Time Complexity:
//   constructor: O(V log V)
//   query: O(1)
// Memory Complexity: O(V)
// Tested:
//   https://cses.fi/problemset/task/1139/
template <class T> struct CountDistinctSubtree {
  struct R {
    using Data = int; using Lazy = int; vector<Data> A;
    static Data qdef() { return 0; }
    R(const vector<Data> &A) : A(A) {}
    void update(int l, int, const Lazy &val) { A[l] += val; }
    Data query(int l, int r) { return A[r] - (l == 0 ? 0 : A[l - 1]); }
  };
  LCA<> lca; SubtreeQueries<R, false> sbtr;
  int query(int v) { return sbtr.querySubtree(v); }
  template <class Forest>
  CountDistinctSubtree(const Forest &G, const vector<T> &A,
                       const vector<int> &roots = vector<int>())
      : lca(G, roots), sbtr(G, vector<int>(A.size(), 0), roots) {
    int V = G.size(); vector<T> temp = A; sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    vector<int> C(V), st(temp.size() + 1, 0), ind(V);
    for (int v = 0; v < V; v++)
      st[C[v] = lower_bound(temp.begin(), temp.end(), A[v]) - temp.begin()]++;
    partial_sum(st.begin(), st.end(), st.begin());
    for (int v = 0; v < V; v++) ind[--st[C[v]]] = v;
    for (int i = 0; i < int(temp.size()); i++)
      for (int j = st[i]; j < st[i + 1]; j++) {
        sbtr.updateVertex(ind[j], 1); if (j + 1 < st[i + 1])
          sbtr.updateVertex(lca.lca(ind[j], ind[j + 1]), -1);
      }
    partial_sum(sbtr.ops.A.begin(), sbtr.ops.A.end(), sbtr.ops.A.begin());
  }
  template <class Forest>
  CountDistinctSubtree(const Forest &G, const vector<T> &A, int rt)
      : CountDistinctSubtree(G, A, vector<int>{rt}) {}
};
