#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/PolicyBasedDataStructures.h"
using namespace std;
using namespace __gnu_pbds;

// Computes the global minimum cut for a weighted graph
// A cut is a partition of the vertices into two nonempty subsets
// A crossing edge is an edge with endpoints in both subsets
// The cost of a cut is the sum of the weights of the crossing edges
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the weight of the edges in the graph
// Constructor Arguments:
//   V: the number of vertices in the graph
//   INF: a value for infinity
// Fields:
//   cut: vector of booleans representing which side of the cut each vertex
//     is on
//   cutWeight: the weight of the global minimum cut
// Functions:
//   addEdge(v, w, weight): adds an undirected edge between vertices v
//     and w, with a weight of weight
//   globalMinCut(): returns the global minimum cut of the current graph
//     and updates the cut array
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(V)
//   addEdge: O(1)
//   globalMinCut: O(V (V + E) log V)
// Memory Complexity: O(V^2 + E)
// Tested:
//   https://dmoj.ca/problem/checkercut
//   https://hackerrank.com/contests/w37/challenges/two-efficient-teams/problem
template <class T> struct StoerWagnerGlobalMinCut {
  struct Edge {
    int to; T weight; int rev;
    Edge(int to, T weight, int rev) : to(to), weight(weight), rev(rev) {}
  };
  int V; vector<vector<Edge>> G; vector<bool> cut; T cutWeight, INF;
  void addEdge(int v, int w, T weight) {
    if (v == w) return;
    G[v].emplace_back(w, weight, int(G[w].size()));
    G[w].emplace_back(v, weight, int(G[v].size()) - 1);
  }
  StoerWagnerGlobalMinCut(int V, T INF = numeric_limits<T>::max())
      : V(V), G(V), cut(V, false), cutWeight(INF), INF(INF) {}
  T globalMinCut() {
    vector<vector<Edge>> H = G; fill(cut.begin(), cut.end(), false);
    cutWeight = INF; vector<int> par(V); iota(par.begin(), par.end(), 0);
    for (int phase = V - 1; phase > 0; phase--) {
      vector<T> W(V, T()); pbdsheap<pair<T, int>> PQ;
      vector<typename decltype(PQ)::point_iterator> ptr(V, PQ.end());
      for (int v = 1; v < V; v++) if (par[v] == v)
        ptr[v] = PQ.push(make_pair(W[v], v));
      for (auto &&e : H[0]) if (ptr[e.to] != PQ.end())
        PQ.modify(ptr[e.to], make_pair(W[e.to] += e.weight, e.to));
      for (int i = 0, v, last = 0; i < phase; i++, last = v) {
        T w; tie(w, v) = PQ.top(); PQ.pop(); ptr[v] = PQ.end();
        if (i == phase - 1) {
          if (cutWeight > w) {
            cutWeight = w; for (int x = 0; x < V; x++) cut[x] = par[x] == v;
          }
          fill(W.begin(), W.end(), T());
          for (auto &&e : H[v]) W[e.to] += e.weight;
          for (auto &&e : H[last]) {
            e.weight += W[e.to]; H[e.to][e.rev].weight += W[e.to];
            W[e.to] = T();
          }
          for (auto &&e : H[v]) if (W[e.to] != T()) {
            H[e.to][e.rev].to = last; H[e.to][e.rev].rev = H[last].size();
            H[last].emplace_back(e.to, e.weight, e.rev);
          }
          H[v].clear();
          for (int x = 0; x < V; x++) if (par[x] == v) par[x] = last;
        } else {
          for (auto &&e : H[v]) if (ptr[e.to] != PQ.end())
            PQ.modify(ptr[e.to], make_pair(W[e.to] += e.weight, e.to));
        }
      }
    }
    return cutWeight;
  }
};
