#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Computes the minimum spanning tree (or forest) using Boruvka's algorithm
// Vertices are 0-indexed
// Constructor Arguments:
//   V: number of vertices in the graph
//   edges: a vector of tuples in the form (v, w, weight) representing
//     an undirected edge in the graph between vertices v and w with
//     weight of weight
// Fields:
//   mstWeight: the weight of the mst
//   mstEdges: a vector of tuples of the edges in the mst
// In practice, has a small constant, slower than Prim and Kruskal
// Time Complexity:
//   constructor: O(V + E log V)
// Memory Complexity: O(V)
// Tested:
//   Stress Tested
//   https://open.kattis.com/problems/minspantree
template <class T> struct BoruvkaMST {
  using Edge = tuple<int, int, T>;
  T mstWeight; vector<Edge> mstEdges; UnionFind uf;
  BoruvkaMST(int V, const vector<Edge> &edges) : mstWeight(), uf(V) {
    for (int t = 1; t < V && int(mstEdges.size()) < V - 1; t *= 2) {
      vector<int> closest(V, -1); for (int e = 0; e < int(edges.size()); e++) {
        int v = uf.find(get<0>(edges[e])), w = uf.find(get<1>(edges[e]));
        if (v == w) continue;
        if (closest[v] == -1 || get<2>(edges[e]) < get<2>(edges[closest[v]]))
          closest[v] = e;
        if (closest[w] == -1 || get<2>(edges[e]) < get<2>(edges[closest[w]]))
          closest[w] = e;
      }
      for (int v = 0; v < V; v++) if (closest[v] != -1
          && uf.join(get<0>(edges[closest[v]]), get<1>(edges[closest[v]]))) {
        mstEdges.push_back(edges[closest[v]]);
        mstWeight += get<2>(edges[closest[v]]);
      }
    }
  }
};
