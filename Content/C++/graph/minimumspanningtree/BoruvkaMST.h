#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Computes the minimum spanning tree using Boruvka's algorithm
// Vertices are 0-indexed
// Constructor Arguments:
//   V: number of vertices in the graph
//   edges: a vector of KruskalMST::Edge of the edges in the graph
// Members:
//   mstWeight: the weight of the mst
//   mstEdges: a vector of KruskalMST::Edge of the edges in the mst
// In practice, has a small constant, slower than Prim and Kruskal
// Time Complexity:
//   constructor: O(V + E log V)
// Memory Complexity: O(V) additional memory
// Tested:
//   Stress Tested
//   https://open.kattis.com/problems/minspantree
template <class T> struct BoruvkaMST {
  struct Edge {
    int v, w; T weight;
    Edge(int v, int w, T weight) : v(v), w(w), weight(weight) {}
  };
  T mstWeight; vector<Edge> mstEdges; UnionFind uf;
  BoruvkaMST(int V, const vector<Edge> &edges) : mstWeight(), uf(V) {
    for (int t = 1; t < V && int(mstEdges.size()) < V - 1; t *= 2) {
      vector<int> closest(V, -1); for (int e = 0; e < int(edges.size()); e++) {
        int v = uf.find(edges[e].v), w = uf.find(edges[e].w);
        if (v == w) continue;
        if (closest[v] == -1 || edges[e].weight < edges[closest[v]].weight)
          closest[v] = e;
        if (closest[w] == -1 || edges[e].weight < edges[closest[w]].weight)
          closest[w] = e;
      }
      for (int v = 0; v < V; v++) if (closest[v] != -1
          && uf.join(edges[closest[v]].v, edges[closest[v]].w)) {
        mstEdges.push_back(edges[closest[v]]);
        mstWeight += edges[closest[v]].weight;
      }
    }
  }
};
