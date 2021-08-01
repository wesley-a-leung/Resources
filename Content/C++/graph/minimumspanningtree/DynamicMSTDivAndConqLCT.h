#pragma once
#include <bits/stdc++.h>
#include "IncrementalMSTUndo.h"
#include "../../queries/FIFOSetDivAndConq.h"
using namespace std;

// Support queries for the minimum spanning tree, after edges have been
//   added or removed, using divide and conquer and a Link Cut Tree
// Template Arguments:
//   U: the type of the weight of the edges
// Constructor Arguments:
//   V: number of vertices in the graph
//   NEG_INF: a value for negative infinity of type U
// Fields:
//   ans: a vector of type U with the minimum spanning tree for each query 
// Functions:
//   addEdge(v, w, weight): adds an edge between vertices v and w with
//     a weight of weight
//   removeEdge(v, w, weight): removes an edge between vertices v and w with
//     a weight of weight, assuming such an edge exists
//   addMstQuery(v, w): adds a query asking for the current minimum spanning
//     tree (or forest)
//   solveQueries(): solves all queries asked so far
// In practice, has a moderate constant, slower than
//   DynamicMSTDivAndConqUnionFind
// Time Complexity:
//   constructor: O(V)
//   addEdge, removeEdge, addMstQuery: O(1)
//   solveQueries: O(V + Q log Q log V)
// Memory Complexity: O(V + Q) for Q edge additions/removals and queries
// Tested:
//   https://dmoj.ca/problem/ccoprep4p3
template <class U> struct DynamicMSTDivAndConqLCT {
  struct S {
    using T = tuple<int, int, U>; using R = U;
    struct Q { bool _; };
    IncrementalMSTUndo<U> imst; vector<int> stk;
    S(int V, U NEG_INF) : imst(V, NEG_INF) {}
    void add(const T &v) { imst.addEdge(get<0>(v), get<1>(v), get<2>(v)); }
    void saveOnStack() { stk.push_back(imst.history.size()); }
    void rollback() {
      while (int(imst.history.size()) > stk.back()) imst.undo();
      stk.pop_back();
    }
    R query(const Q &) { return imst.mstWeight; }
  };
  int V; U NEG_INF; FIFOSetDivAndConq<S> s; vector<U> &ans = s.ans;
  DynamicMSTDivAndConqLCT(int V, U NEG_INF = numeric_limits<U>::lowest())
      : V(V), NEG_INF(NEG_INF) {}
  void addEdge(int v, int w, U weight) {
    if (v > w) swap(v, w);
    s.addElement(make_tuple(v, w, weight));
  }
  void removeEdge(int v, int w, U weight) {
    if (v > w) swap(v, w);
    s.removeElement(make_tuple(v, w, weight));
  }
  void addMstQuery() { typename S::Q q; s.addQuery(q); }
  void solveQueries() { s.solveQueries(V, NEG_INF); }
};
