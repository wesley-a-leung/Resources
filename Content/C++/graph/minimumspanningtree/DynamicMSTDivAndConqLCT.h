#pragma once
#include <bits/stdc++.h>
#include "IncrementalMSTUndo.h"
using namespace std;

// Support queries for the minimum spanning tree, after edges have been
//   added or removed, using divide and conquer and a Link Cut Tree
// Template Arguments:
//   T: the type of the weight of the edges
// Constructor Arguments:
//   V: number of vertices in the graph
//   NEG_INF: a value for negative infinity of type T
// Fields:
//   ans: a vector of type T with the minimum spanning tree for each query 
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
template <class T> struct DynamicMSTDivAndConqLCT {
  int V; T NEG_INF;
  vector<tuple<int, int, int, T, int>> queries; vector<T> ans;
  DynamicMSTDivAndConqLCT(int V, T NEG_INF = numeric_limits<T>::lowest())
      : V(V), NEG_INF(NEG_INF) {}
  void addEdge(int v, int w, T weight) {
    if (v > w) swap(v, w);
    queries.emplace_back(0, v, w, weight, -1);
  }
  void removeEdge(int v, int w, T weight) {
    if (v > w) swap(v, w);
    queries.emplace_back(1, v, w, weight, -1);
  }
  void addMstQuery() {
    queries.emplace_back(2, -1, -1, T(), queries.size());
  }
  void solveQueries() {
    vector<tuple<int, int, T>> edges; int Q = queries.size(); edges.reserve(Q);
    for (auto &&q : queries) if (get<0>(q) == 0)
      edges.emplace_back(get<1>(q), get<2>(q), get<3>(q));
    sort(edges.begin(), edges.end()); vector<int> last(edges.size(), INT_MAX);
    for (int i = 0; i < Q; i++) {
      int t, v, w, _; T weight; tie(t, v, w, weight, _) = queries[i];
      tuple<int, int, T> e(v, w, weight); if (t == 0) {
        int j = lower_bound(edges.begin(), edges.end(), e) - edges.begin();
        get<4>(queries[i]) = last[j]; last[j] = i;
      } else if (t == 1) {
        int j = lower_bound(edges.begin(), edges.end(), e) - edges.begin();
        int temp = get<4>(queries[get<4>(queries[i]) = last[j]]);
        get<4>(queries[last[j]]) = i; last[j] = temp;
      }
    }
    IncrementalMSTUndo<T> imst(V, NEG_INF); ans.clear(); ans.reserve(Q);
    function<void(int, int)> dc = [&] (int l, int r) {
      if (l == r) {
        int t, v, w, _; T weight; tie(t, v, w, weight, _) = queries[l];
        if (t == 2) ans.push_back(imst.mstWeight);
        return;
      }
      int m = l + (r - l) / 2, curSize = imst.history.size();
      for (int i = m + 1; i <= r; i++) if (get<4>(queries[i]) < l)
        imst.addEdge(get<1>(queries[i]), get<2>(queries[i]),
                      get<3>(queries[i]));
      dc(l, m); while (int(imst.history.size()) > curSize) imst.undo();
      for (int i = l; i <= m; i++) if (get<4>(queries[i]) > r)
        imst.addEdge(get<1>(queries[i]), get<2>(queries[i]),
                      get<3>(queries[i]));
      dc(m + 1, r); while (int(imst.history.size()) > curSize) imst.undo();
    };
    if (Q > 0) dc(0, Q - 1);
  }
};
