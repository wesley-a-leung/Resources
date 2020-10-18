#pragma once
#include <bits/stdc++.h>
using namespace std;

// Dijkstra's single source shortest path algorithm for weighted graphs
//   without negative weights
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   G: a generic weighted graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of pair<int, T> with weights of type T)
//       size() const: returns the number of vertices in the graph
//   s: a single source vertex
//   src: a vector of source vertices
//   INF: a value for infinity
// Fields:
//   dist: vector of shortest distance from the closest source vertex to each
//     vertex, or INF if unreachable
//   par: the parent vertex for each vertex in the shortest path tree, or
//     -1 if there is no parent
// Functions:
//   getPath(v): returns the list of directed edges on the path from the
//     closest source vertex to vertex v
// In practice, has a small constant
// Time Complexity:
//   constructor: O((V + E) log E)
//   getPath: O(V)
// Memory Complexity: O(V)
// Tested:
//   Stress Tested
//   https://judge.yosupo.jp/problem/shortest_path
//   https://open.kattis.com/problems/shortestpath1
//   https://dmoj.ca/problem/sssp
template <class T> struct DijkstraSSSP {
  using Edge = tuple<int, int, T>; vector<T> dist; vector<int> par; T INF;
  template <class WeightedGraph>
  DijkstraSSSP(const WeightedGraph &G, const vector<int> &srcs,
               T INF = numeric_limits<T>::max())
      : dist(G.size(), INF), par(G.size(), -1), INF(INF) {
    std::priority_queue<pair<T, int>, vector<pair<T, int>>,
                        greater<pair<T, int>>> PQ;
    for (int s : srcs) PQ.emplace(dist[s] = T(), s);
    while (!PQ.empty()) {
      T d = PQ.top().first; int v = PQ.top().second; PQ.pop();
      if (d > dist[v]) continue;
      for (auto &&e : G[v]) if (dist[e.first] > dist[v] + e.second)
        PQ.emplace(dist[e.first] = dist[par[e.first] = v] + e.second, e.first);
    }
  }
  template <class WeightedGraph> DijkstraSSSP(const WeightedGraph &G, int s,
                                              T INF = numeric_limits<T>::max())
      : DijkstraSSSP(G, vector<int>{s}, INF) {}
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v] != -1; v = par[v])
      path.emplace_back(par[v], v, dist[v] - dist[par[v]]);
    reverse(path.begin(), path.end()); return path;
  }
};
