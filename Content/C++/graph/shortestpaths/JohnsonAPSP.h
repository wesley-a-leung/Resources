#pragma once
#include <bits/stdc++.h>
#include "../search/TransitiveClosureSCC.h"
using namespace std;

// Johnson's all pairs shortest path algorithm for weighted graphs
//   with negative weights
// Able to detect negative cycles
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   G: a generic weighted graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of pair<int, T> with weights of type T)
//       size() const: returns the number of vertices in the graph
//   MAXV: the maximum number of vertices in the graph
//   INF: a value for infinity, must be negatable
// Fields:
//   dist: a vector of vectors of the shortest distance between each pair
//     of vertices, INF if there is no path, -INF if the shortest path
//     has no lower bound
//   par: a vector of vectors of the parent vertex for each vertex in the
//     shortest path tree for each source vertex (par[v][w] is the parent
//     of vertex w in the shortest path tree from vertex v), or -1 if there is
//     no parent
//   hasNegativeCycle: a boolean that is true if there is a negative cycle
//     in the graph and false otherwise
// Functions:
//   getPath(v, w): returns the list of directed edges on the path from
//     vertex v to vertex w
// In practice, has a small constant
// Time Complexity:
//   constructor: O(VE log E)
//   getPath: O(V)
// Memory Complexity: O(V + E + MAXV V / 64)
// Tested:
//   https://dmoj.ca/problem/apsp
template <class T, const int MAXV> struct JohnsonAPSP {
  using Edge = tuple<int, int, T>; int V; vector<vector<T>> dist;
  vector<vector<int>> par; T INF; bool hasNegativeCycle;
  template <class WeightedGraph>
  JohnsonAPSP(const WeightedGraph &G, T INF = numeric_limits<T>::max())
      : V(G.size()), dist(V, vector<T>(V, INF)), par(V, vector<int>(V, -1)),
        INF(INF), hasNegativeCycle(false) {
    bool hasNegativeWeight = false; for (int v = 0; v < V; v++)
      for (auto &&e : G[v]) hasNegativeWeight |= e.second < T();
    vector<bool> inNegCyc(V, false); vector<T> h(V, T());
    vector<int> id; vector<bitset<MAXV>> neg; if (hasNegativeWeight) {
      TransitiveClosureSCC<MAXV> tc(G); id = tc.scc.id;
      vector<bool> compInNegCyc(tc.dp.size(), false);
      for (int i = 0; i < V - 1; i++) for (int v = 0; v < V; v++)
        for (auto &&e : G[v]) if (id[v] == id[e.first])
          h[e.first] = min(h[e.first], h[v] + e.second);
      for (int v = 0; v < V; v++) for (auto &&e : G[v])
        if (id[v] == id[e.first] && h[e.first] > h[v] + e.second)
          compInNegCyc[id[v]] = true;
      for (int v = 0; v < V; v++)
        hasNegativeCycle |= (inNegCyc[v] = compInNegCyc[id[v]]);
      fill(h.begin(), h.end(), T());
      for (int i = 0; i < V - 1; i++) for (int v = 0; v < V; v++)
        for (auto &&e : G[v]) if (!inNegCyc[v] && !inNegCyc[e.first])
          h[e.first] = min(h[e.first], h[v] + e.second);
      vector<bool> vis(tc.dp.size(), false); neg.resize(vis.size());
      function<void(int)> dfs = [&] (int v) {
        vis[v] = 1; for (int e = tc.st[v]; e < tc.st[v + 1]; e++) {
          int w = tc.DAG[e].second; if (!vis[w]) dfs(w);
          neg[v] |= neg[w];
        }
        if (compInNegCyc[v]) neg[v] = tc.dp[v];
      };
      for (int i = 0; i < int(neg.size()); i++) if (!vis[i]) dfs(i);
    }
    for (int s = 0; s < V; s++) {
      if (!inNegCyc[s]) {
        std::priority_queue<pair<T, int>, vector<pair<T, int>>,
                            greater<pair<T, int>>> PQ;
        PQ.emplace(dist[s][s] = T(), s); while (!PQ.empty()) {
          T d = PQ.top().first; int v = PQ.top().second; PQ.pop();
          if (d > dist[s][v]) continue;
          for (auto &&e : G[v]) {
            int w = e.first; T weight = e.second + h[v] - h[w];
            if (!inNegCyc[w] && dist[s][w] > dist[s][v] + weight)
              PQ.emplace(dist[s][w] = dist[s][par[s][w] = v] + weight, w);
          }
        }
      }
      if (hasNegativeWeight) for (int v = 0; v < V; v++)
        if (neg[id[s]][id[v]]) dist[s][v] = -INF;
    }
    if (hasNegativeWeight) for (int v = 0; v < V; v++)
      for (int w = 0; w < V; w++) if (dist[v][w] != INF && dist[v][w] != -INF)
        dist[v][w] = dist[v][w] - h[v] + h[w];
  }
  vector<Edge> getPath(int v, int w) {
    vector<Edge> path; for (; par[v][w] != -1; w = par[v][w])
      path.emplace_back(par[v][w], w, dist[v][w] - dist[v][par[v][w]]);
    reverse(path.begin(), path.end()); return path;
  }
};
