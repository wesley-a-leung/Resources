#pragma once
#include <bits/stdc++.h>
using namespace std;

// Dijkstra's single source shortest path algorithm for weighted graphs
//   without negative weights
// Vertices are 0-indexed
// contructor accepts a generic weighted graph data structure
//   with the [] operator (const) defined to iterate over the adjacency list
//   (which is a list of pair<int, T> with weights of type T), as well as a
//   member function size() (const) that returns the number of vertices in the
//   graph, a single source vertex, or a list of sources,
//   and a value for infinity
// dist[v] is the distance from the closest source vertex to vertex v,
//   or INF if unreachable
// par[v] is a pair of the parent vertex for vertex v in the dijkstra tree,
//   and the weight of the edge to the parent
// getPath(v) returns the list of edges on the path from the closest source
//   vertex to vertex v
// In practice, constructor has a small constant
// Time Complexity:
//   constructor: O((V + E) log E) if a regular priority queue is used
//   getPath: O(V)
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/shortest_path
//   https://open.kattis.com/problems/shortestpath1
//   https://dmoj.ca/problem/sssp
template <class T> struct SSSPDijkstra {
  vector<T> dist; vector<pair<int, T>> par; T INF;
  template <class WeightedGraph>
  SSSPDijkstra(const WeightedGraph &G, const vector<int> &srcs,
               T INF = numeric_limits<T>::max())
      : dist(G.size(), INF), par(G.size(), make_pair(-1, INF)), INF(INF) {
    std::priority_queue<pair<T, int>, vector<pair<T, int>>,
                        greater<pair<T, int>>> PQ;
    for (int s : srcs) PQ.emplace(dist[s] = 0, s);
    while (!PQ.empty()) {
      T d = PQ.top().first; int v = PQ.top().second; PQ.pop();
      if (d > dist[v]) continue;
      for (auto &&e : G[v]) if (dist[e.first] > dist[v] + e.second) {
        par[e.first] = make_pair(v, e.second);
        PQ.emplace(dist[e.first] = dist[v] + e.second, e.first);
      }
    }
  }
  template <class WeightedGraph> SSSPDijkstra(const WeightedGraph &G, int s,
                                              T INF = numeric_limits<T>::max())
      : SSSPDijkstra(G, vector<int>(1, s), INF) {}
  struct Edge {
    int from, to; T weight;
    Edge(int from, int to, T weight) : from(from), to(to), weight(weight) {}
  };
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v].first != -1; v = par[v].first)
      path.emplace_back(par[v].first, v, par[v].second);
    reverse(path.begin(), path.end()); return path;
  }
};

// Classical Dijkstra's single source shortest path algorithm for
//   weighted graphs without negative weights
// Vertices are 0-indexed
// contructor accepts a generic weighted graph data structure
//   with the [] operator (const) defined to iterate over the adjacency list
//   (which is a list of pair<int, T> with weights of type T), as well as a
//   member function size() (const) that returns the number of vertices in the
//   graph, a single source vertex, or a list of sources,
//   and a value for infinity
// dist[v] is the distance from the closest source vertex to vertex v,
//   or INF if unreachable
// par[v] is a pair of the parent vertex for vertex v in the dijkstra tree,
//   and the weight of the edge to the parent
// getPath(v) returns the list of edges on the path from the closest source
//   vertex to vertex v
// In practice, constructor has a small constant
// Time Complexity:
//   constructor: O(V^2 + E)
//   getPath: O(V)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/sssp
template <class T> struct SSSPDijkstraClassical {
  vector<T> dist; vector<pair<int, T>> par; T INF;
  template <class WeightedGraph>
  SSSPDijkstraClassical(const WeightedGraph &G, const vector<int> &srcs,
                        T INF = numeric_limits<T>::max())
      : dist(G.size(), INF), par(G.size(), make_pair(-1, INF)), INF(INF) {
    vector<bool> done(G.size(), false); for (int s : srcs) dist[s] = 0;
    for (int i = 0; i < int(G.size()) - 1; i++) {
      int v = -1; for (int w = 0; w < int(G.size()); w++)
        if (!done[w] && (v == -1 || dist[v] > dist[w])) v = w;
      if (dist[v] == INF) break;
      done[v] = true;
      for (auto &&e : G[v]) if (dist[e.first] > dist[v] + e.second) {
        par[e.first] = make_pair(v, e.second);
        dist[e.first] = dist[v] + e.second;
      }
    }
  }
  template <class WeightedGraph>
  SSSPDijkstraClassical(const WeightedGraph &G, int s,
                        T INF = numeric_limits<T>::max())
      : SSSPDijkstraClassical(G, vector<int>(1, s), INF) {}
  struct Edge {
    int from, to; T weight;
    Edge(int from, int to, T weight) : from(from), to(to), weight(weight) {}
  };
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v].first != -1; v = par[v].first)
      path.emplace_back(par[v].first, v, par[v].second);
    reverse(path.begin(), path.end()); return path;
  }
};
