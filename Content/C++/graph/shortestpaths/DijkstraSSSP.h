#pragma once
#include <bits/stdc++.h>
using namespace std;

// Dijkstra's single source shortest path algorithm for weighted graphs
//   without negative weights
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   G: a generic graph data structure (weighted or unweighted)
//     with the [] operator (const) defined to iterate over the adjacency list
//     (which is a list of ints for an unweighted graph, or a list of
//     pair<int, T> for a weighted graph with weights of type T), as well as a
//     member function size() (const) that returns the number of vertices
//     in the graph
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
// In practice, the constructor has a small constant
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
      : DijkstraSSSP(G, vector<int>(1, s), INF) {}
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v] != -1; v = par[v])
      path.emplace_back(par[v], v, dist[v] - dist[par[v]]);
    reverse(path.begin(), path.end()); return path;
  }
};

// Classical Dijkstra's single source shortest path algorithm for
//   weighted graphs without negative weights
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   G: a generic graph data structure (weighted or unweighted)
//     with the [] operator (const) defined to iterate over the adjacency list
//     (which is a list of ints for an unweighted graph, or a list of
//     pair<int, T> for a weighted graph with weights of type T), as well as a
//     member function size() (const) that returns the number of vertices
//     in the graph
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
// In practice, the constructor has a small constant
// Time Complexity:
//   constructor: O(V^2 + E)
//   getPath: O(V)
// Memory Complexity: O(V)
// Tested:
//   Stress Tested
//   https://dmoj.ca/problem/sssp
template <class T> struct ClassicalDijkstraSSSP {
  using Edge = tuple<int, int, T>; vector<T> dist; vector<int> par; T INF;
  template <class WeightedGraph>
  ClassicalDijkstraSSSP(const WeightedGraph &G, const vector<int> &srcs,
                        T INF = numeric_limits<T>::max())
      : dist(G.size(), INF), par(G.size(), -1), INF(INF) {
    vector<bool> done(G.size(), false); for (int s : srcs) dist[s] = T();
    for (int i = 0; i < int(G.size()) - 1; i++) {
      int v = -1; for (int w = 0; w < int(G.size()); w++)
        if (!done[w] && (v == -1 || dist[v] > dist[w])) v = w;
      if (dist[v] == INF) break;
      done[v] = true;
      for (auto &&e : G[v]) if (dist[e.first] > dist[v] + e.second)
        dist[e.first] = dist[par[e.first] = v] + e.second;
    }
  }
  template <class WeightedGraph>
  ClassicalDijkstraSSSP(const WeightedGraph &G, int s,
                        T INF = numeric_limits<T>::max())
      : ClassicalDijkstraSSSP(G, vector<int>(1, s), INF) {}
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v] != -1; v = par[v])
      path.emplace_back(par[v], v, dist[v] - dist[par[v]]);
    reverse(path.begin(), path.end()); return path;
  }
};
