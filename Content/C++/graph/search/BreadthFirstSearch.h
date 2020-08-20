#pragma once
#include <bits/stdc++.h>
using namespace std;

// Breadth First Traversal of a graph (weighted or unweighted)
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   G: a generic graph data structure (weighted or unweighted)
//     Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints for an unweighted graph, or a list of
//         pair<int, T> for a weighted graph with weights of type T)
//       size() const: returns the number of vertices in the graph
//   s: a single source vertex
//   src: a vector of source vertices
//   INF: a value for infinity
// Fields:
//   dist: vector of distance from the closest source vertex to each vertex,
//     or INF if unreachable, and is also the shortest distance for
//     an unweighted graph
//   par: the parent vertex for each vertex in the breadth first search tree,
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
//   https://www.spoj.com/problems/BITMAP/
//   https://dmoj.ca/problem/ddrp3
template <class T = int> struct BFS {
  using Edge = tuple<int, int, T>; vector<T> dist; vector<int> par; T INF;
  int getTo(int e) { return e; }
  T getWeight(int) { return 1; }
  int getTo(const pair<int, T> &e) { return e.first; }
  T getWeight(const pair<int, T> &e) { return e.second; }
  template <class Graph> BFS(const Graph &G, const vector<int> &srcs,
                             T INF = numeric_limits<T>::max())
      : dist(G.size(), INF), par(G.size(), -1), INF(INF) {
    vector<int> q(G.size(), -1); int front = 0, back = 0;
    for (int s : srcs) dist[q[back++] = s] = T();
    while (front < back) {
      int v = q[front++]; for (auto &&e : G[v]) {
        int w = getTo(e); if (dist[w] == INF)
          dist[q[back++] = w] = dist[par[w] = v] + getWeight(e);
      }
    }
  }
  template <class Graph> BFS(const Graph &G, int s,
                             T INF = numeric_limits<T>::max())
      : BFS(G, vector<int>{s}, INF) {}
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v] != -1; v = par[v])
      path.emplace_back(par[v], v, dist[v] - dist[par[v]]);
    reverse(path.begin(), path.end()); return path;
  }
};
