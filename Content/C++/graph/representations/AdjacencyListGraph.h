#pragma once
#include <bits/stdc++.h>
using namespace std;

// Adjacency List representation of a graph
// Vertices are 0-indexed
// Constructor Arguments:
//   V: the number of vertices in the graph
// Functions:
//   addDiEdge(from, to): adds a directed edge from the vertex from,
//     to the vertex to
//   addBiEdge(v, w): adds a bidirectional edge between vertices v and w
//   operator [v]: returns a reference to a list of vertices adjacent to v
//   size(): returns the number of vertices in the graph
// In practice, addBiEdge, addDiEdge have a moderate constant, operator []
//   has a small constant
// Graph construction is slower than static graphs and linked lists
// Graph traveral is faster than static graphs, but slower than linked lists
// Uses more memory than static graphs and linked lists
// Time Complexity:
//   constructor: O(V)
//   addBiEdge, addDiEdge: O(1) amortized
//   operator [], size: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/lca
struct AdjacencyListGraph : public vector<vector<int>> {
  AdjacencyListGraph(int V) : vector<vector<int>>(V) {}
  void addDiEdge(int from, int to) { at(from).push_back(to); }
  void addBiEdge(int v, int w) { addDiEdge(v, w); addDiEdge(w, v); }
};

// Adjacency List representation of a weighted graph
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the weighted graph
// Constructor Arguments:
//   V: the number of vertices in the weighted graph
// Functions:
//   addDiEdge(from, to, weight): adds a directed edge from the vertex from,
//     to the vertex to, with a weight of weight
//   addBiEdge(v, w, weight): adds a bidirectional edge between vertices v
//     and w, with a weight of weight
//   operator [v]: returns a reference to a list of edges incident to v
//   size(): returns the number of vertices in the graph
// In practice, addBiEdge, addDiEdge have a moderate constant, operator []
//   has a small constant
// Graph construction is slower than static graphs and linked lists
// Graph traveral is faster than static graphs, but slower than linked lists
// Uses more memory than static graphs and linked lists
// Time Complexity:
//   constructor: O(V)
//   addBiEdge, addDiEdge: O(1) amortized
//   operator [], size: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/rte16s3
template <class T>
struct AdjacencyListWeightedGraph : public vector<vector<pair<int, T>>> {
  AdjacencyListWeightedGraph(int V) : vector<vector<pair<int, T>>>(V) {}
  void addDiEdge(int from, int to, T weight) {
    this->at(from).emplace_back(to, weight);
  }
  void addBiEdge(int v, int w, T weight) {
    addDiEdge(v, w, weight); addDiEdge(w, v, weight);
  }
};
