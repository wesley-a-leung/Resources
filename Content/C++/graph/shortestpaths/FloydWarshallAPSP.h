#pragma once
#include <bits/stdc++.h>
using namespace std;

// Floyd Warshall's all pairs shortest path algorithm for weighted graphs
//   with negative weight
// Able to detect negative cycles
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   matrix: a V x V matrix containing the minimum weight of a directed edge
//     between two vertices, INF if an edge doesn't exist
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
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(V^3)
//   getPath: O(V)
// Memory Complexity: O(V^2)
// Tested:
//   Fuzz and Stress Tested
//   https://open.kattis.com/problems/allpairspath
template <class T> struct FloydWarshallAPSP {
  using Edge = tuple<int, int, T>; vector<vector<T>> dist;
  vector<vector<int>> par; T INF; bool hasNegativeCycle;
  FloydWarshallAPSP(const vector<vector<T>> &matrix,
                    T INF = numeric_limits<T>::max())
      : dist(matrix), par(dist.size(), vector<int>(dist.size(), -1)),
        INF(INF), hasNegativeCycle(false) {
    int V = dist.size(); for (int v = 0; v < V; v++) {
      for (int w = 0; w < V; w++) if (dist[v][w] < INF) par[v][w] = v;
      dist[v][v] = min(T(), dist[v][v]); par[v][v] = -1;
    }
    for (int u = 0; u < V; u++) for (int v = 0; v < V; v++)
      if (dist[v][u] < INF) for (int w = 0; w < V; w++)
        if (dist[u][w] < INF && dist[v][w] > dist[v][u] + dist[u][w]) {
          dist[v][w] = dist[v][u] + dist[u][w]; par[v][w] = par[u][w];
        }
    for (int u = 0; u < V; u++) for (int v = 0; v < V; v++)
      for (int w = 0; w < V; w++)
        if (dist[w][w] < T() && dist[u][w] < INF && dist[w][v] < INF) {
          dist[u][v] = -INF; hasNegativeCycle = true; break;
        }
  }
  vector<Edge> getPath(int v, int w) {
    vector<Edge> path; for (; par[v][w] != -1; w = par[v][w])
      path.emplace_back(par[v][w], w, dist[v][w] - dist[v][par[v][w]]);
    reverse(path.begin(), path.end()); return path;
  }
};
