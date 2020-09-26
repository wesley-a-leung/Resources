#pragma once
#include <bits/stdc++.h>
using namespace std;

// Supports queries for whether a graph component is bipartite
//   after edges have been added using Union Find by size supporting undos
// Vertices are 0-indexed
//   V: the number of vertices in the graph
// Fields:
//   UF: a vector of integers representing the parent of each vertex in the
//     tree, or the negative of the size of the connected component if that
//     vertex is a root
//   P: a vector of booleans representing the parity of the path to the root
//     of that connected component
//   B: a vector of booleans representing whether the component rooted at the
//     vertex is bipartite for root vertices, undefined otherwise
//   cnt: the current number of connected components
//   bipartiteGraph: a boolean indicating whether the current graph is
//     bipartite or not
//   history: a vector of tuples storing the history of all addEdge calls
// Functions:
//   find(v): returns a pair containing the root of the connected component and
//     the parity of the path from the root to vertex v
//   addEdge(v, w): adds an edge between vertices v and w
//   undo(): undoes the last edge added by popping from the history stack
//   connected(v, w): returns true if v and w are in the same
//     connected component, false otherwise
//   getSize(v): returns the size of the connected component containing
//     vertex v
//   componentBipartite(v): returns whether the connected component containing
//     vertex v is bipartite or not
//   color(v): returns the color of vertex v for one
//     possible coloring of the graph, assuming the component is bipartite
//   pathParity(v, w): return the parity of the path from
//     v to w (false if even number of edges, true if odd), assuming the
//     component is bipartite and v and w are connected
// Time Complexity:
//   constructor: O(V)
//   find, addEdge, connected, getSize: O(log V)
//   componentBipartite, color, pathParity: O(log V)
//   undo: O(1)
// Memory Complexity: O(V + Q) for Q calls to addEdge
// Tested:
//   https://www.spoj.com/problems/BUGLIFE/
//   https://cses.fi/problemset/task/1668
//   https://codeforces.com/contest/813/problem/F
struct SemiDynamicBipartiteUndo {
  vector<int> UF; vector<bool> P, B; int cnt; bool bipartiteGraph;
  vector<tuple<int, int, int, bool, bool>> history;
  SemiDynamicBipartiteUndo(int V)
      : UF(V, -1), P(V, false), B(V, true), cnt(V), bipartiteGraph(true) {}
  pair<int, bool> find(int v) {
    bool p = P[v]; for (; UF[v] >= 0; p ^= P[v = UF[v]]);
    return make_pair(v, p);
  }
  void addEdge(int v, int w) {
    bool pv, pw; tie(v, pv) = find(v); tie(w, pw) = find(w); if (v == w) {
      history.emplace_back(v, w, 0, bipartiteGraph, B[v]);
      bipartiteGraph &= (B[v] = B[v] & (pv ^ pw)); return;
    }
    if (UF[v] > UF[w]) { swap(v, w); swap(pv, pw); }
    history.emplace_back(v, w, UF[w], P[w], B[v]);
    UF[v] += UF[w]; UF[w] = v; P[w] = pv ^ pw ^ 1; B[v] = B[v] & B[w]; cnt--;
  }
  void undo() {
    int v, w, ufw; bool pw, bv; tie(v, w, ufw, pw, bv) = history.back();
    history.pop_back(); B[v] = bv; if (ufw == 0) bipartiteGraph = pw;
    else { UF[w] = ufw; UF[v] -= UF[w]; P[w] = pw; cnt++; }
  }
  bool connected(int v, int w) { return find(v).first == find(w).first; }
  int getSize(int v) { return -UF[find(v).first]; }
  bool componentBipartite(int v) { return B[find(v).first]; }
  bool color(int v) { return find(v).second; }
  bool pathParity(int v, int w) { return color(v) ^ color(w); }
};
