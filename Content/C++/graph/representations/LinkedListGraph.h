#pragma once
#include <bits/stdc++.h>
using namespace std;

// Adjacency List representation of a graph using linked list,
//   implemented with fixed size arrays if reserveEdges is called beforehand
// Vertices are 0-indexed
// Constructor Arguments:
//   V: the number of vertices in the graph
// Functions:
//   reserveDiEdges(maxEdges): reserves space for maxEdges directed edges
//     (bidirectional edges take up twice as much space)
//   addDiEdge(from, to): adds a directed edge from the vertex from,
//     to the vertex to
//   addBiEdge(v, w): adds a bidirectional edge between vertices v and w
//   operator [v]: returns a struct with the begin() and end() defined to
//     iterate over the vertices adjacent to vertex v
//   size(): returns the number of vertices in the graph
// In practice, addBiEdge, addDiEdge have a small constant, operator []
//   has a moderate constant
// Graph construction is faster than static graphs and adjacency lists
// Graph traveral is slower than static graphs and adjacency lists
// Uses less memory than static graphs and adjacency lists
// Time Complexity:
//   constructor: O(V)
//   addDiEdge: O(1) amortized
//   operator [], size: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/lca
struct LinkedListGraph {
  vector<int> HEAD, TO, NXT; LinkedListGraph(int V) : HEAD(V, -1) {}
  void reserveDiEdges(int maxEdges) {
    TO.reserve(maxEdges); NXT.reserve(maxEdges);
  }
  void addDiEdge(int from, int to) {
    NXT.push_back(HEAD[from]); HEAD[from] = int(TO.size()); TO.push_back(to);
  }
  void addBiEdge(int v, int w) { addDiEdge(v, w); addDiEdge(w, v); }
  struct Iterator {
    const LinkedListGraph &G; int i;
    Iterator(const LinkedListGraph &G, int i) : G(G), i(i) {}
    Iterator &operator ++ () { i = G.NXT[i]; return *this; }
    int operator * () const { return G.TO[i]; }
    bool operator != (const Iterator &it) const { return i != it.i; }
  };
  struct Adj {
    const LinkedListGraph &G; int v;
    Adj(const LinkedListGraph &G, int v) : G(G), v(v) {}
    const Iterator begin() const { return Iterator(G, G.HEAD[v]); }
    const Iterator end() const { return Iterator(G, -1); }
  };
  const Adj operator [] (int v) const { return Adj(*this, v); }
  int size() const { return HEAD.size(); }
};

// Adjacency List representation of a weighted graph using linked list,
//   implemented with fixed size arrays if reserveEdges is called beforehand
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the weighted graph
// Constructor Arguments:
//   V: the number of vertices in the weighted graph
// Functions:
//   reserveDiEdges(maxEdges): reserves space for maxEdges directed edges
//     (bidirectional edges take up twice as much space)
//   addDiEdge(from, to, weight): adds a directed edge from the vertex from,
//     to the vertex to, with a weight of weight
//   addBiEdge(v, w, weight): adds a bidirectional edge between vertices v
//     and w, with a weight of weight
//   operator [v]: returns a struct with the begin() and end() defined to
//     iterate over the edges incident to vertex v
//   size(): returns the number of vertices in the graph
// In practice, addBiEdge, addDiEdge have a small constant, operator []
//   has a moderate constant
// Graph construction is faster than static graphs and adjacency lists
// Graph traveral is slower than static graphs and adjacency lists
// Uses less memory than static graphs and adjacency lists
// Time Complexity:
//   constructor: O(V)
//   addBiEdge, addDiEdge: O(1) amortized
//   operator [], size: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/rte16s3
template <class T> struct LinkedListWeightedGraph {
  vector<int> HEAD, TO, NXT; vector<T> WEIGHT;
  LinkedListWeightedGraph(int V) : HEAD(V, -1) {}
  void reserveDiEdges(int maxEdges) {
    TO.reserve(maxEdges); NXT.reserve(maxEdges); WEIGHT.reserve(maxEdges);
  }
  void addDiEdge(int from, int to, T weight) {
    NXT.push_back(HEAD[from]); HEAD[from] = int(TO.size());
    TO.push_back(to); WEIGHT.push_back(weight);
  }
  void addBiEdge(int v, int w, T weight) {
    addDiEdge(v, w, weight); addDiEdge(w, v, weight);
  }
  struct Iterator {
    const LinkedListWeightedGraph &G; int i;
    Iterator(const LinkedListWeightedGraph &G, int i) : G(G), i(i) {}
    Iterator &operator ++ () { i = G.NXT[i]; return *this; }
    pair<int, T> operator * () const {
      return make_pair(G.TO[i], G.WEIGHT[i]);
    }
    bool operator != (const Iterator &it) const { return i != it.i; }
  };
  struct Adj {
    const LinkedListWeightedGraph &G; int v;
    Adj(const LinkedListWeightedGraph &G, int v) : G(G), v(v) {}
    const Iterator begin() const { return Iterator(G, G.HEAD[v]); }
    const Iterator end() const { return Iterator(G, -1); }
  };
  const Adj operator [] (int v) const { return Adj(*this, v); }
  int size() const { return HEAD.size(); }
};
