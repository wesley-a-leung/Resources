#pragma once
#include <bits/stdc++.h>
#include "../search/TopologicalOrder.h"
using namespace std;

// Computes the shortest path (based on the comparator) on a directed
//   acyclic graph
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
//   Cmp: the comparator to compare two distances; less<T> will compute
//       the shortest path while greater<T> will compute the longest path
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   G: a generic directed acyclic graph structure (weighted or unweighted)
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints for an unweighted graph, or a list of
//         pair<int, T> for a weighted graph with weights of type T)
//       size() const: returns the number of vertices in the graph
//   s: a single source vertex
//   src: a vector of source vertices
//   INF: a value for infinity for shortest path, or negative infinity
//     for longest path; (Cmp()(INF, x)) must return false for all
//     values of x
// Fields:
//   dist: vector of distance from the closest source vertex to each vertex,
//     or INF if unreachable, and is also the shortest distance for
//     an unweighted graph
//   par: the parent vertex for each vertex in the shortest path tree,
//     or -1 if there is no parent
// Functions:
//   getPath(v): returns the list of directed edges on the path from the
//     closest source vertex to vertex v
// In practice, the constructor has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
//   getPath: O(V)
// Memory Complexity: O(V)
// Tested:
//   https://atcoder.jp/contests/dp/tasks/dp_g
template <class T, class Cmp = less<T>> struct DAGSP {
  using Edge = tuple<int, int, T>; vector<T> dist; vector<int> par; T INF;
  TopologicalOrder ord;
  int getTo(int e) { return e; }
  T getWeight(int) { return 1; }
  int getTo(const pair<int, T> &e) { return e.first; }
  T getWeight(const pair<int, T> &e) { return e.second; }
  template <class DAG> DAGSP(const DAG &G, const vector<int> &srcs, T INF)
      : dist(G.size(), INF), par(G.size(), -1), INF(INF), ord(G) {
    for (int s : srcs) dist[s] = T();
    for (int v : ord.ord) for (auto &&e : G[v]) {
      int w = getTo(e); T weight = getWeight(e);
      if (dist[v] != INF && Cmp()(dist[v] + weight, dist[w]))
        dist[w] = dist[par[w] = v] + weight;
    }
  }
  template <class DAG> DAGSP(const DAG &G, int s, T INF)
      : DAGSP(G, vector<int>{s}, INF) {}
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v] != -1; v = par[v])
      path.emplace_back(par[v], v, dist[v] - dist[par[v]]);
    reverse(path.begin(), path.end()); return path;
  }
};
