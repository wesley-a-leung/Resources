#pragma once
#include <bits/stdc++.h>
#include "PushRelabelMaxFlow.h"
using namespace std;

// Given a list of weighted edges representing an undirected flow graph,
//   compute a tree such that the max flow/min cut between any pair of vertices
//   is given by the minimum weight edge between the two vertices
// Template Arguments:
//   T: the type of the weight of the edges
// Constructor Arguments:
//   V: number of vertices in the graph
//   edges: a vector of tuples in the form (v, w, weight) representing
//     an undirected edge in the graph between vertices v and w with
//     weight of weight
// Fields:
//   treeEdges: a vector of tuples of the edges in the Gomory-Hu Tree
// In practice, has a very small constant
// Time Complexity:
//   constructor: O(V^3 sqrt E), much faster in practice
// Memory Complexity: O(V + E)
// Tested:
//   https://codeforces.com/gym/101480/problem/J
template <class T> struct GomoryHu {
  using Edge = tuple<int, int, T>; vector<Edge> treeEdges;
  GomoryHu(int V, const vector<Edge> &edges, T EPS = T(1e-9)) {
    PushRelabelMaxFlow<FlowEdge<T>> mf(V, EPS); for (auto &&e : edges)
      mf.addEdge(get<0>(e), get<1>(e), get<2>(e), get<2>(e));
    vector<int> par(V, 0); for (int i = 1; i < V; i++) {
      for (int v = 0; v < V; v++) for (auto &&e : mf.G[v]) e.resCap = e.cap;
      treeEdges.emplace_back(i, par[i], mf.getFlow(i, par[i]));
      for (int j = i + 1; j < V; j++)
        if (par[j] == par[i] && mf.cut[j]) par[j] = i;
    }
  }
};
