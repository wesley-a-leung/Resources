#pragma once
#include <bits/stdc++.h>
using namespace std;

// Static Graph implemented with fixed size arrays
//   if reserveEdges is called beforehand
// build must be called before the graph can be used, and edges cannot be
//   added afterwards
// Vertices are 0-indexed
// In practice, addBiEdge and addDiEdge have a small constant, build has a
//   moderate constant, and operator [] has a very small constant
// Graph construction is faster than adjacency lists, but slower than
//   linked lists
// Graph traveral is faster than adjacency lists and linked lists
// Uses less memory than adjacency lists, but more memory than linked lists
// Time Complexity:
//   constructor: O(V)
//   addDiEdge: O(1) amortized
//   build: O(V + E)
//   operator [], size: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/lca
struct StaticGraph {
  vector<int> ST, TO, A, B; StaticGraph(int V) : ST(V + 1, 0) {}
  void reserveDiEdges(int maxEdges) {
    TO.reserve(maxEdges); A.reserve(maxEdges); B.reserve(maxEdges);
  }
  void addDiEdge(int from, int to) {
    ST[from]++; A.push_back(from); B.push_back(to);
  }
  void addBiEdge(int v, int w) { addDiEdge(v, w); addDiEdge(w, v); }
  void build() {
    partial_sum(ST.begin(), ST.end(), ST.begin()); TO = B;
    for (int e = 0; e < int(A.size()); e++) TO[--ST[A[e]]] = B[e];
  }
  struct Iterator {
    const StaticGraph &G; int i;
    Iterator(const StaticGraph &G, int i) : G(G), i(i) {}
    Iterator &operator ++ () { i++; return *this; }
    int operator * () const { return G.TO[i]; }
    bool operator != (const Iterator &it) const { return i != it.i; }
  };
  struct Adj {
    const StaticGraph &G; int v;
    Adj(const StaticGraph &G, int v) : G(G), v(v) {}
    const Iterator begin() const { return Iterator(G, G.ST[v]); }
    const Iterator end() const { return Iterator(G, G.ST[v + 1]); }
  };
  const Adj operator [] (int v) const { return Adj(*this, v); }
  int size() const { return int(ST.size()) - 1; }
};

// Static Weighted Graph implemented with fixed size arrays
//   if reserveEdges is called beforehand
// build must be called before the graph can be used, and edges cannot be
//   added afterwards
// Vertices are 0-indexed
// In practice, addBiEdge and addDiEdge have a small constant, build has a
//   moderate constant, and operator [] has a very small constant
// Graph construction is faster than adjacency lists, but slower than
//   linked lists
// Graph traveral is faster than adjacency lists and linked lists
// Uses less memory than adjacency lists, but more memory than linked lists
// Time Complexity:
//   constructor: O(V)
//   addBiEdge, addDiEdge: O(1) amortized
//   build: O(V + E)
//   operator [], size: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/rte16s3
template <class T> struct StaticWeightedGraph {
  vector<int> ST, TO, A, B; vector<T> C, WEIGHT;
  StaticWeightedGraph(int V) : ST(V + 1, 0) {}
  void reserveDiEdges(int maxEdges) {
    TO.reserve(maxEdges); A.reserve(maxEdges); B.reserve(maxEdges);
  }
  void addDiEdge(int from, int to, T weight) {
    ST[from]++; A.push_back(from); B.push_back(to); C.push_back(weight);
  }
  void addBiEdge(int v, int w, T weight) {
    addDiEdge(v, w, weight); addDiEdge(w, v, weight);
  }
  void build() {
    partial_sum(ST.begin(), ST.end(), ST.begin()); TO = B; WEIGHT = C;
    for (int e = 0; e < int(A.size()); e++) {
      TO[--ST[A[e]]] = B[e]; WEIGHT[ST[A[e]]] = C[e];
    }
  }
  struct Iterator {
    const StaticWeightedGraph &G; int i;
    Iterator(const StaticWeightedGraph &G, int i) : G(G), i(i) {}
    Iterator &operator ++ () { i++; return *this; }
    pair<int, T> operator * () const {
      return make_pair(G.TO[i], G.WEIGHT[i]);
    }
    bool operator != (const Iterator &it) const { return i != it.i; }
  };
  struct Adj {
    const StaticWeightedGraph &G; int v;
    Adj(const StaticWeightedGraph &G, int v) : G(G), v(v) {}
    const Iterator begin() const { return Iterator(G, G.ST[v]); }
    const Iterator end() const { return Iterator(G, G.ST[v + 1]); }
  };
  const Adj operator [] (int v) const { return Adj(*this, v); }
  int size() const { return int(ST.size()) - 1; }
};
