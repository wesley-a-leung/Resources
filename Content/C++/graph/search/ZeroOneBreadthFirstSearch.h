#pragma once
#include <bits/stdc++.h>
using namespace std;

// Breadth First Traversal of a graph where all edge weights are 0 or 1
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
// Fields:
//   dist: vector of the shortest distance from the closest source vertex to
//     each vertex, or INT_MAX if unreachable
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
//   https://www.codechef.com/problems/REVERSE
//   https://atcoder.jp/contests/abc176/tasks/abc176_d
struct ZeroOneBFS {
  using Edge = tuple<int, int, int>; vector<int> dist, par;
  template <class WeightedGraph>
  ZeroOneBFS(const WeightedGraph &G, const vector<int> &srcs)
      : dist(G.size(), INT_MAX), par(G.size(), -1) {
    vector<int> q(G.size()), stk(G.size()); int front = 0, back = 0, top = 0;
    for (int s : srcs) dist[stk[top++] = s] = 0;
    while (top > 0 || front < back) {
      int v = top > 0 ? stk[--top] : q[front++]; for (auto &&e : G[v]) {
        int w = e.first; if (dist[w] > dist[v] + e.second) {
          (e.second == 0 ? stk[top++] : q[back++]) = w;
          dist[w] = dist[par[w] = v] + e.second;
        }
      }
    }
  }
  template <class WeightedGraph> ZeroOneBFS(const WeightedGraph &G, int s)
      : ZeroOneBFS(G, vector<int>{s}) {}
  vector<Edge> getPath(int v) {
    vector<Edge> path; for (; par[v] != -1; v = par[v])
      path.emplace_back(par[v], v, dist[v] - dist[par[v]]);
    reverse(path.begin(), path.end()); return path;
  }
};
