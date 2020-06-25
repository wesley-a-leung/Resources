#pragma once
#include <bits/stdc++.h>
using namespace std;

// Static Graph implemented with fixed size arrays
//   if reserveEdges is called beforehand
// build must be called before the graph can be used, and edges cannot be
//   added afterwards
// Vertices are 0-indexed
// In practice, addDiEdge has a small constant, build has a moderate constant,
//   and operator [] has a very small constant
// Graph construction is faster than adjacency lists, but slower than
//   linked lists
// Graph traveral is faster than adjacency lists and linked lists
// Uses less memory than adjacency lists, but more memory than linked lists
// Time Complexity:
//   constructor: O(V)
//   addDiEdge: O(1) amortized
//   build: O(V + E)
//   operator []: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/lca
struct GraphStatic {
  int V; vector<int> ST, TO, A, B; GraphStatic(int V) : V(V), ST(V + 1, 0) {}
  void reserveEdges(int maxEdges) {
    TO.reserve(maxEdges); A.reserve(maxEdges); B.reserve(maxEdges);
  }
  void addDiEdge(int from, int to) {
    ST[from]++; A.push_back(from); B.push_back(to);
  }
  void build() {
    partial_sum(ST.begin(), ST.end(), ST.begin()); TO = B;
    for (int e = 0; e < int(A.size()); e++) TO[--ST[A[e]]] = B[e];
  }
  struct Iterator {
    const GraphStatic &G; int i;
    Iterator(const GraphStatic &G, int i) : G(G), i(i) {}
    Iterator &operator ++ () { i++; return *this; }
    int operator * () const { return G.TO[i]; }
    bool operator != (const Iterator &it) const { return i != it.i; }
  };
  struct Adj {
    const GraphStatic &G; int v;
    Adj(const GraphStatic &G, int v) : G(G), v(v) {}
    const Iterator begin() const { return Iterator(G, G.ST[v]); }
    const Iterator end() const { return Iterator(G, G.ST[v + 1]); }
  };
  const Adj operator [] (int v) const { return Adj(*this, v); }
};

// Static Weighted Graph implemented with fixed size arrays
//   if reserveEdges is called beforehand
// build must be called before the graph can be used, and edges cannot be
//   added afterwards
// Vertices are 0-indexed
// In practice, addDiEdge has a small constant, build has a moderate constant,
//   and operator [] has a very small constant
// Graph construction is faster than adjacency lists, but slower than
//   linked lists
// Graph traveral is faster than adjacency lists and linked lists
// Uses less memory than adjacency lists, but more memory than linked lists
// Time Complexity:
//   constructor: O(V)
//   addDiEdge: O(1) amortized
//   build: O(V + E)
//   operator []: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/rte16s3
template <class T> struct WeightedGraphStatic {
  int V; vector<int> ST, TO, A, B; vector<T> C, WEIGHT;
  WeightedGraphStatic(int V) : V(V), ST(V + 1, 0) {}
  void reserveEdges(int maxEdges) {
    TO.reserve(maxEdges); A.reserve(maxEdges); B.reserve(maxEdges);
  }
  void addDiEdge(int from, int to, T weight) {
    ST[from]++; A.push_back(from); B.push_back(to); C.push_back(weight);
  }
  void build() {
    partial_sum(ST.begin(), ST.end(), ST.begin()); TO = B; WEIGHT = C;
    for (int e = 0; e < int(A.size()); e++) {
      TO[--ST[A[e]]] = B[e]; WEIGHT[ST[A[e]]] = C[e];
    }
  }
  struct Iterator {
    const WeightedGraphStatic &G; int i;
    Iterator(const WeightedGraphStatic &G, int i) : G(G), i(i) {}
    Iterator &operator ++ () { i++; return *this; }
    pair<int, T> operator * () const {
      return make_pair(G.TO[i], G.WEIGHT[i]);
    }
    bool operator != (const Iterator &it) const { return i != it.i; }
  };
  struct Adj {
    const WeightedGraphStatic &G; int v;
    Adj(const WeightedGraphStatic &G, int v) : G(G), v(v) {}
    const Iterator begin() const { return Iterator(G, G.ST[v]); }
    const Iterator end() const { return Iterator(G, G.ST[v + 1]); }
  };
  const Adj operator [] (int v) const { return Adj(*this, v); }
};
