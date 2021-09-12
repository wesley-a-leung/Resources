#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFind.h"
using namespace std;

// Support online queries for the number of bridges in a graph, after edges
//   have been added
// Constructor Arguments:
//   V: the number of vertices in the graph
// Fields:
//   bridgeCnt: the current number of bridges in the graph
//   uf1: a UnionFind data structure representing the 1-edge connected
//     components
//   uf2: a UnionFind data structure representing the 2-edge connected
//     components
// Functions:
//   addEdge(v, w): adds an edge between vertices v and w
// Time Complexity:
//   constructor: O(V)
//   addEdge: O(log V) amortized
// Memory Complexity: O(V)
// Tested:
//   https://codeforces.com/gym/100551/problem/B
struct IncrementalBridges {
  UnionFind uf1, uf2; vector<int> par, mn, vis; int stamp, bridgeCnt;
  IncrementalBridges(int V)
      : uf1(V), uf2(V), par(V, -1), mn(V), vis(V, -1),
        stamp(-1), bridgeCnt(0) {
    iota(mn.begin(), mn.end(), 0);
  }
  void addEdge(int v, int w) {
    if (uf2.connected(v, w)) return;
    if (uf1.connected(v, w)) {
      stamp++; int lca = -1; for (int x = v, y = w;; swap(x, y)) if (x != -1) {
        if (vis[x = mn[uf2.find(x)]] == stamp) { lca = x; break; }
        vis[x] = stamp; x = par[x];
      }
      for (int h = 0; h < 2; h++, swap(v, w))
        for (v = mn[uf2.find(v)]; v != lca;) {
          int p = mn[uf2.find(par[v])], pp = par[p]; uf2.join(v, p);
          par[v = mn[uf2.find(v)] = p] = pp; bridgeCnt--;
        }
    } else {
      if (uf1.getSize(v) < uf1.getSize(w)) swap(v, w);
      for (int p = -1, last = v, x = w; x != -1; last = x, x = p) {
        p = par[x = mn[uf2.find(x)]]; par[x] = last;
      }
      bridgeCnt++; uf1.join(v, w);
    }
  }
};
