#pragma once
#include <bits/stdc++.h>
using namespace std;

// Bellman Ford's single source shortest path algorithm for weighted graphs
//   with negative weights
// Able to detect negative cycles
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   V: the number of vertices in the graph
//   edges: a vector of tuples in the form (v, w, weight) representing
//     a directed edge in the graph from v to w with weight of weight
//   s: a single source vertex
//   src: a vector of source vertices
//   INF: a value for infinity, must be negatable
// Fields:
//   dist: vector of shortest distance from the closest source vertex to each
//     vertex, or INF if unreachable
//   par: the parent vertex for each vertex in the shortest path tree, or
//     -1 if there is no parent
//   hasNegativeCycle: a boolean that is true if there is a negative cycle
//     in the graph and false otherwise
// Functions:
//   getPath(v): returns the list of directed edges on the path from the
//     closest source vertex to vertex v
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(E (V + log E))
//   getPath: O(V)
// Memory Complexity: O(V + E)
// Tested:
//   Stress Tested
//   https://open.kattis.com/problems/shortestpath3
//   https://dmoj.ca/problem/sssp
template <class T> struct BellmanFordSSSP {
  using Edge = tuple<int, int, T>; vector<T> dist; vector<int> par; T INF;
  bool hasNegativeCycle;
  BellmanFordSSSP(int V, vector<Edge> edges, const vector<int> &srcs,
                  T INF = numeric_limits<T>::max())
      : dist(V, INF), par(V, -1), INF(INF), hasNegativeCycle(false) {
    sort(edges.begin(), edges.end()); for (int s : srcs) dist[s] = T();
    for (int i = 0; i < V - 1; i++) for (auto &&e : edges) {
      int v, w; T weight; tie(v, w, weight) = e;
      if (dist[v] < INF && dist[w] > dist[v] + weight)
        dist[w] = dist[par[w] = v] + weight;
    }
    for (bool inCycle = true; inCycle;) {
      inCycle = false; for (auto &&e : edges) {
        int v, w; T weight; tie(v, w, weight) = e;
        if (dist[v] < INF && dist[w] > -INF
            && (dist[v] <= -INF || dist[w] > dist[v] + weight)) {
          dist[w] = -INF; inCycle = hasNegativeCycle = true;
        }
      }
    }
  }
  BellmanFordSSSP(int V, const vector<Edge> &edges, int s,
                  T INF = numeric_limits<T>::max())
      : BellmanFordSSSP(V, edges, vector<int>{s}, INF) {}
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v] != -1; v = par[v])
      path.emplace_back(par[v], v, dist[v] - dist[par[v]]);
    reverse(path.begin(), path.end()); return path;
  }
};
