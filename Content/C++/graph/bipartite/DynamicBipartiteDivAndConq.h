#pragma once
#include <bits/stdc++.h>
#include "IncrementalBipartiteUndo.h"
#include "../../queries/LIFOSetDivAndConq.h"
using namespace std;

// Support offline queries on connected components and bipartiteness, after
//   edges have been added or removed, using divide and conquer
// Constructor Arguments:
//   V: the number of vertices in the graph
// Fields:
//   ans: a vector of integers with the answer for
//     each query (1 is true, 0 is false for boolean queries)
// Functions:
//   addEdge(v, w): adds an edge between vertices v and w
//   removeEdge(v, w): removes an edge between vertices v and w, assuming
//     an edge exists
//   addConnectedQuery(v, w): adds a query asking whether v and w are in the
//     same connected component
//   addSizeQuery(v): adds a query asking for the number of vertices in the
//     same connected component as vertex v
//   addCntQuery(): adds a query asking for the number of connected components
//   addComponentBipartiteQuery(v): adds a query asking for whether the
//     connected component containing vertex v is bipartite
//   addBipartiteGraphQuery(): adds a query asking if the graph is bipartite
//   addColorQuery(v): adds a query asking for the color of vertex v for one
//     possible coloring of the graph, assuming the component is bipartite
//   addPathParityQuery(v, w): adds a query asking for the parity of the path
//     from v to w (false if even number of edges, true if odd), assuming the
//     component is bipartite and v and w are connected
//   solveQueries(): solves all queries asked so far
// In practice, has a small constant
// Time Complexity:
//   constructor: O(1)
//   addEdge, removeEdge, addConnectedQuery, addSizeQuery, addCntQuery: O(1)
//   addComponentBipartiteQuery, addBipartiteGraphQuery: O(1)
//   addColorQuery, addPathParity: O(1)
//   solveQueries: O(V + Q log Q log V)
// Memory Complexity: O(V + Q) for Q edge additions/removals and queries
// Tested:
//   https://codeforces.com/contest/813/problem/F
struct DynamicBipartiteDivAndConq {
  struct S {
    using T = pair<int, int>; using R = int;
    struct Q { int type, v, w; };
    IncrementalBipartiteUndo uf;
    S(int V) : uf(V) {}
    void push(const T &e) { uf.addEdge(e.first, e.second); }
    void pop() { uf.undo(); }
    R query(const Q &q) {
      if (q.type == 1) return uf.connected(q.v, q.w);
      else if (q.type == 2) return uf.getSize(q.v);
      else if (q.type == 3) return uf.cnt;
      else if (q.type == 4) return uf.componentBipartite(q.v);
      else if (q.type == 5) return uf.bipartiteGraph;
      else if (q.type == 6) return uf.color(q.v);
      else return uf.pathParity(q.v, q.w);
    }
  };
  int V; LIFOSetDivAndConq<S> s; vector<int> &ans = s.ans;
  DynamicBipartiteDivAndConq(int V) : V(V) {}
  void addEdge(int v, int w) {
    if (v > w) swap(v, w);
    s.addElement(make_pair(v, w));
  }
  void removeEdge(int v, int w) {
    if (v > w) swap(v, w);
    s.removeElement(make_pair(v, w));
  }
  void addConnectedQuery(int v, int w) {
    S::Q q; q.type = 1; q.v = v; q.w = w; s.addQuery(q);
  }
  void addSizeQuery(int v) {
    S::Q q; q.type = 2; q.v = q.w = v; s.addQuery(q);
  }
  void addCntQuery() { S::Q q; q.type = 3; q.v = q.w = -1; s.addQuery(q); }
  void addComponentBipartiteQuery(int v) {
    S::Q q; q.type = 4; q.v = q.w = v; s.addQuery(q);
  }
  void addBipartiteGraphQuery() {
    S::Q q; q.type = 5; q.v = q.w = -1; s.addQuery(q);
  }
  void addColorQuery(int v) {
    S::Q q; q.type = 6; q.v = q.w = v; s.addQuery(q);
  }
  void addPathParityQuery(int v, int w) {
    S::Q q; q.type = 7; q.v = v; q.w = w; s.addQuery(q);
  }
  void solveQueries() { s.solveQueries(V); }
};
