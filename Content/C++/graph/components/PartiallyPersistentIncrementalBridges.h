#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
#include "../../datastructures/unionfind/PartiallyPersistentUnionFind.h"
using namespace std;

// Supports persistent queries on 2-edge connected component after
//   edges have been added
// Constructor Arguments:
//   V: number of vertices in the graph
//   edges: a vector of pairs in the form (v, w) representing
//     an undirected edge in the graph between vertices v and w in the order
//     of the list
// Functions:
//   find(t, v): inherited from PartiallyPersistentUnionFind, finds an
//     arbitrary root of the 2-edge connected component containing vertex v,
//     after the edge at index t in edges is added
//   connected(t, v, w): inherited from PartiallyPersistentUnionFind,
//     returns true if v and w are in the 2-edge connected component, after the
//     edge at index t in edges is added, returns false otherwise
//   getSize(t, v): inherited from PartiallyPersistentUnionFind, returns the
//     size of the 2-edge connected component containing vertex v, after the
//     edge at index t in edges is added
//   getCnt(t): inherited from PartiallyPersistentUnionFind, returns the number
//     of 2-edge connected components, after the edge at index t in
//     edges is added
//   getFirst(v, w): inherited from PartiallyPersistentUnionFind, returns
//     the index of the first edge in edges that vertices v and w are in the
//     same 2-edge connected component, -1 if v == w and curTime + 1 if they
//     are never in the same 2-edge connected component
//   getBridgeCnt(t): returns the number of bridges in the graph, after the
//     edge at index t in edges is added
// In practice, constructor, find, connected, getSize, getCnt,
//   getBridgeCnt have a small constant, getFirst has a very small constant
// Time Complexity:
//   constructor: O(V alpha V + E log V)
//   find, join, connected, getSize, getCnt: O(log V)
//   getFirst: O(log V log E)
//   getBridgeCnt: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/mcco17d1p3
struct PartiallyPersistentIncrementalBridges
    : public PartiallyPersistentUnionFind {
  vector<int> bridgeCnt;
  PartiallyPersistentIncrementalBridges(int V,
                                        const vector<pair<int, int>> &edges)
      : PartiallyPersistentUnionFind(V), bridgeCnt(edges.size()) {
    int E = edges.size(); vector<bool> inForest(E);
    vector<int> par(V, -1), dep(V), st(V + 1, 0), mn(V), to(E * 2);
    function<void(int, int, int)> dfs = [&] (int v, int prev, int d) {
      par[v] = prev; dep[v] = d; for (int e = st[v]; e < st[v + 1]; e++) {
        int w = to[e]; if (w != prev) dfs(w, v, d + 1);
      }
    };
    UnionFind forest(V); for (int i = 0; i < E; i++)
      inForest[i] = forest.join(edges[i].first, edges[i].second);
    iota(mn.begin(), mn.end(), 0); for (int i = 0; i < E; i++)
      if (inForest[i]) { st[edges[i].first]++; st[edges[i].second]++; }
    partial_sum(st.begin(), st.end(), st.begin());
    for (int i = 0; i < E; i++) if (inForest[i]) {
      int v, w; tie(v, w) = edges[i]; to[--st[v]] = w; to[--st[w]] = v;
    }
    for (int v = 0; v < V; v++) if (par[v] == -1) dfs(v, -1, 0);
    for (int i = 0, curBridgeCnt = 0; i < E; i++) {
      if (inForest[i]) curBridgeCnt++;
      else {
        int v, w; tie(v, w) = edges[i];
        for (v = mn[find(i, v)], w = mn[find(i, w)]; v != w;) {
          if (dep[v] < dep[w]) swap(v, w);
          int p = mn[find(i, par[v])]; join(v, p); v = mn[find(i, v)] = p;
          curTime--; curBridgeCnt--;
        }
      }
      bridgeCnt[i] = curBridgeCnt; curTime++;
    }
  }
  int getBridgeCnt(int t) { return bridgeCnt[t]; }
};
