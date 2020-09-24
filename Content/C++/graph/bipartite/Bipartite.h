#pragma once
#include <bits/stdc++.h>
using namespace std;

// Determines whether an undirected graph is bipartite, or whether it has
//   an odd cycle
// Vertices are 0-indexed
// Constructor Arguments:
//   G: a generic undirected graph structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the graph
// Fields:
//   bipartite: a boolean indicating whether the graph is bipartite or not
//   color: a vector of booleans for one possible bipartite coloring
//   oddCycle: a vector of the vertices in an odd cycle of the graph, empty
//     if the graph is bipartite
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V + E)
// Memory Complexity: O(V)
// Tested:
//   Stress Tested
//   https://www.spoj.com/problems/BUGLIFE/
//   https://cses.fi/problemset/task/1669
struct Bipartite {
  int V; bool bipartite; vector<bool> color; vector<int> oddCycle;
  template <class Graph> Bipartite(const Graph &G)
      : V(G.size()), bipartite(true), color(V, false) {
    vector<int> to(G.size(), -2), q(G.size()), stk(G.size()); int top = 0;
    for (int s = 0; s < V && bipartite; s++) if (to[s] == -2) {
      int front = 0, back = 0; to[q[back++] = s] = -1; while (front < back) {
        int v = q[front++]; for (int w : G[v]) {
          if (to[w] == -2) color[q[back++] = w] = !color[to[w] = v];
          else if (color[w] == color[v]) {
            bipartite = false; int x = v, y = w; while (x != y) {
              x = to[stk[top++] = x]; oddCycle.push_back(y); y = to[y];
            }
            stk[top++] = x; while (top > 0) oddCycle.push_back(stk[--top]);
            oddCycle.push_back(w); return;
          }
        }
      }
    }
  }
};
