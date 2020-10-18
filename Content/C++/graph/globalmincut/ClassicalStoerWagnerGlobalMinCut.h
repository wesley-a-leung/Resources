#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
using namespace std;
using namespace __gnu_pbds;

// Computes the global minimum cut for a weighted graph
// A cut is a partition of the vertices into two nonempty subsets
// A crossing edges is an edge with endpoints in both subsets
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
//   constructor: O(V^2)
//   addEdge: O(1)
//   globalMinCut: O(V^2)
// Memory Complexity: O(V^2)
// Tested:
//   https://dmoj.ca/problem/checkercut
//   https://hackerrank.com/contests/w37/challenges/two-efficient-teams/problem
template <class T> struct ClassicalStoerWagnerGlobalMinCut {
  int V; vector<vector<T>> G; vector<bool> cut; T cutWeight, INF;
  void addEdge(int v, int w, T weight) {
    if (v == w) return;
    G[v][w] += weight; G[w][v] += weight;
  } 
  ClassicalStoerWagnerGlobalMinCut(int V, T INF = numeric_limits<T>::max())
      : V(V), G(V, vector<T>(V, T())), cut(V, false),
        cutWeight(INF), INF(INF) {}
  T globalMinCut() {
    vector<vector<T>> H = G; fill(cut.begin(), cut.end(), false);
    cutWeight = INF; vector<int> par(V); iota(par.begin(), par.end(), 0);
    for (int phase = V - 1; phase > 0; phase--) {
      vector<T> W = H[0]; vector<bool> vis(V, true);
      for (int v = 0; v < V; v++) vis[v] = par[v] != v;
      for (int i = 0, v, last = 0; i < phase; i++, last = v) {
        v = -1; for (int w = 1; w < V; w++)
          if (!vis[w] && (v == -1 || W[v] < W[w])) v = w;
        if (i == phase - 1) {
          if (cutWeight > W[v]) {
            cutWeight = W[v]; for (int w = 0; w < V; w++) cut[w] = par[w] == v;
          }
          for (int w = 0; w < V; w++) {
            H[last][w] += H[v][w]; H[w][last] += H[v][w];
            if (par[w] == v) par[w] = last;
          }
        } else {
          vis[v] = true; for (int w = 0; w < V; w++) W[w] += H[v][w];
        }
      }
    }
    return cutWeight;
  }
};
