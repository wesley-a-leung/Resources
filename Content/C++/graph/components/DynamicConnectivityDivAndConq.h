#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFindUndo.h"
#include "../../queries/LIFOSetDivAndConq.h"
using namespace std;

// Support offline queries on connected components, after edges have been
//   added or removed, using divide and conquer
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
//   solveQueries(): solves all queries asked so far
// In practice, has a small constant, faster than DynamicConnectivityLCT and
//   DynamicConnectivityLevelStructure
// Time Complexity:
//   constructor: O(1)
//   addEdge, removeEdge, addConnectedQuery, addSizeQuery, addCntQuery: O(1)
//   solveQueries: O(V + Q log Q log V)
// Memory Complexity: O(V + Q) for Q edge additions/removals and queries
// Tested:
//   https://codeforces.com/gym/100551/problem/A
//   https://codeforces.com/gym/100551/problem/E
struct DynamicConnectivityDivAndConq {
  struct S {
    using T = pair<int, int>; using R = int;
    struct Q { int type, v, w; };
    UnionFindUndo uf;
    S(int V) : uf(V) {}
    void push(const T &e) { uf.join(e.first, e.second); }
    void pop() { uf.undo(); }
    R query(const Q &q) {
      if (q.type == 1) return uf.connected(q.v, q.w);
      else if (q.type == 2) return uf.getSize(q.v);
      else return uf.cnt;
    }
  };
  int V; LIFOSetDivAndConq<S> s; vector<int> &ans = s.ans;
  DynamicConnectivityDivAndConq(int V) : V(V) {}
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
  void solveQueries() { s.solveQueries(V); }
};
