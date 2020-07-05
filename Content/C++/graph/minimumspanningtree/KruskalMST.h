#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Computes the minimum spanning tree using Kruskal's algorithm
// Vertices are 0-indexed
// In practice, has a small constant, faster than Prim and Boruvka
// Time Complexity:
//   constructor: O(V + E log E)
// Memory Complexity: O(V + E)
//   Stress Tested
//   https://open.kattis.com/problems/minspantree
template <class T> struct KruskalMST {
  struct Edge {
    int v, w; T weight;
    Edge(int v, int w, T weight) : v(v), w(w), weight(weight) {}
    bool operator < (const Edge &e) const { return weight < e.weight; }
  };
  T mstWeight; vector<Edge> mstEdges; UnionFind uf;
  KruskalMST(int V, vector<Edge> edges) : mstWeight(), uf(V) {
    sort(edges.begin(), edges.end()); for (auto &&e : edges) {
      if (int(mstEdges.size()) >= V - 1) break;
      if (uf.join(e.v, e.w)) { mstEdges.push_back(e); mstWeight += e.weight; }
    }
  }
};
