#pragma once
#include <bits/stdc++.h>
using namespace std;

// Adjacency List representation of a graph using linked list,
//   implemented with fixed size arrays if reserveEdges is called beforehand
// Vertices are 0-indexed
// In practice, addDiEdge has a small constant, operator []
//   has a moderate constant
// Graph construction is faster than static graphs and adjacency lists
// Graph traveral is slower than static graphs and adjacency lists
// Time Complexity:
//   constructor: O(V)
//   addDiEdge: O(1) amortized
//   operator []: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://judge.yosupo.jp/problem/lca
struct GraphLinkedList {
  int V; vector<int> HEAD, TO, NXT;
  GraphLinkedList(int V) : V(V), HEAD(V, -1) {}
  void reserveEdges(int maxEdges) {
    TO.reserve(maxEdges); NXT.reserve(maxEdges);
  }
  void addDiEdge(int from, int to) {
    NXT.push_back(HEAD[from]); HEAD[from] = int(TO.size()); TO.push_back(to);
  }
  struct Iterator {
    const GraphLinkedList &G; int i;
    Iterator(const GraphLinkedList &G, int i) : G(G), i(i) {}
    Iterator &operator ++ () { i = G.NXT[i]; return *this; }
    int operator * () const { return G.TO[i]; }
    bool operator != (const Iterator &it) const { return i != it.i; }
  };
  struct Adj {
    const GraphLinkedList &G; int v;
    Adj(const GraphLinkedList &G, int v) : G(G), v(v) {}
    const Iterator begin() const { return Iterator(G, G.HEAD[v]); }
    const Iterator end() const { return Iterator(G, -1); }
  };
  const Adj operator [] (int v) const { return Adj(*this, v); }
};

// Adjacency List representation of a weighted graph using linked list,
//   implemented with fixed size arrays if reserveEdges is called beforehand
// Vertices are 0-indexed
// In practice, addDiEdge has a small constant, operator []
//   has a moderate constant
// Graph construction is faster than static graphs and adjacency lists
// Graph traveral is slower than static graphs and adjacency lists
// Time Complexity:
//   constructor: O(V)
//   addBiEdge, addDiEdge: O(1) amortized
//   operator []: O(1)
// Memory Complexity: O(V + E)
// Tested:
//   https://dmoj.ca/problem/rte16s3
template <class T> struct WeightedGraphLinkedList {
  int V; vector<int> HEAD, TO, NXT; vector<T> WEIGHT;
  WeightedGraphLinkedList(int V) : V(V), HEAD(V, -1) {}
  void reserveEdges(int maxEdges) {
    TO.reserve(maxEdges); NXT.reserve(maxEdges); WEIGHT.reserve(maxEdges);
  }
  void addDiEdge(int from, int to, T weight) {
    NXT.push_back(HEAD[from]); HEAD[from] = int(TO.size());
    TO.push_back(to); WEIGHT.push_back(weight);
  }
  struct Iterator {
    const WeightedGraphLinkedList &G; int i;
    Iterator(const WeightedGraphLinkedList &G, int i) : G(G), i(i) {}
    Iterator &operator ++ () { i = G.NXT[i]; return *this; }
    pair<int, T> operator * () const {
      return make_pair(G.TO[i], G.WEIGHT[i]);
    }
    bool operator != (const Iterator &it) const { return i != it.i; }
  };
  struct Adj {
    const WeightedGraphLinkedList &G; int v;
    Adj(const WeightedGraphLinkedList &G, int v) : G(G), v(v) {}
    const Iterator begin() const { return Iterator(G, G.HEAD[v]); }
    const Iterator end() const { return Iterator(G, -1); }
  };
  const Adj operator [] (int v) const { return Adj(*this, v); }
};
