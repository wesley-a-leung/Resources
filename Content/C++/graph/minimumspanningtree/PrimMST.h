#pragma once
#include <bits/stdc++.h>
using namespace std;

// Computes the minimum spanning tree using Prims's algorithm
// Vertices are 0-indexed
// In practice, has a small constant, faster than Boruvka, slower that Kruskal
// Time Complexity:
//   constructor: O((V + E) log E)
// Memory Complexity: O(V + E)
// Tested:
//   Stress Tested
//   https://open.kattis.com/problems/minspantree
template <class T> struct PrimMST {
  struct Edge {
    int v, w; T weight;
    Edge(int v, int w, T weight) : v(v), w(w), weight(weight) {}
  };
  T mstWeight; vector<Edge> mstEdges;
  template <class WeightedGraph> PrimMST(const WeightedGraph &G,
                                         T INF = numeric_limits<T>::max())
      : mstWeight() {
    int V = G.size(); vector<bool> vis(V, false);
    vector<T> cost(V, INF); vector<int> to(V, -1);
    std::priority_queue<pair<T, int>, vector<pair<T, int>>,
                        greater<pair<T, int>>> PQ;
    for (int s = 0; s < V; s++) if (!vis[s]) {
      PQ.emplace(cost[s] = T(), s); while (!PQ.empty()) {
        int v = PQ.top().second; PQ.pop(); if (vis[v]) continue;
        vis[v] = true;
        for (auto &&e : G[v]) if (!vis[e.first] && e.second < cost[e.first]) {
          to[e.first] = v; PQ.emplace(cost[e.first] = e.second, e.first);
        }
      }
    }
    for (int v = 0; v < V; v++) if (to[v] != -1) {
      mstEdges.emplace_back(v, to[v], cost[v]); mstWeight += cost[v];
    }
  }
};
