#pragma once
#include <bits/stdc++.h>
using namespace std;

// Adjacency List representation of a graph
// Vertices are 0-indexed
// In practice, addBiEdge, addDiEdge have a moderate constant, operator []
//   has a small constant
// Graph construction is slower than static graphs and linked lists
// Graph traveral is faster than static graphs, but slower than linked lists
// Uses more memory than static graphs and linked lists
// Time Complexity:
//   constructor: O(V)
//   addBiEdge, addDiEdge: O(1) amortized
//   operator []: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/lca
struct GraphAdjacencyList {
  int V; vector<vector<int>> adj;
  GraphAdjacencyList(int V) : V(V), adj(V) {}
  void addDiEdge(int from, int to) { adj[from].push_back(to); }
  void addBiEdge(int v, int w) { addDiEdge(v, w); addDiEdge(w, v); }
  const vector<int> &operator [] (int v) const { return adj[v]; }
};

// Adjacency List representation of a weighted graph
// Vertices are 0-indexed
// In practice, addBiEdge, addDiEdge have a moderate constant, operator []
//   has a small constant
// Graph construction is slower than static graphs and linked lists
// Graph traveral is faster than static graphs, but slower than linked lists
// Uses more memory than static graphs and linked lists
// Time Complexity:
//   constructor: O(V)
//   addBiEdge, addDiEdge: O(1) amortized
//   operator []: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/rte16s3
template <class T> struct WeightedGraphAdjacencyList {
  int V; vector<vector<pair<int, T>>> adj;
  WeightedGraphAdjacencyList(int V) : V(V), adj(V) {}
  void addDiEdge(int from, int to, T weight) {
    adj[from].emplace_back(to, weight);
  }
  void addBiEdge(int v, int w, T weight) {
    addDiEdge(v, w, weight); addDiEdge(w, v, weight);
  }
  const vector<pair<int, T>> &operator [] (int v) const { return adj[v]; }
};
