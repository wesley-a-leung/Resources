#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFindUndo.h"
using namespace std;

// Support queries for the minimum spanning tree, after edges have been
//   added or removed, using divide and conquer and UnionFindUndo
// Template Arguments:
//   T: the type of the weight of the edges
// Constructor Arguments:
//   V: number of vertices in the graph
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
// In practice, has a moderate constant, faster than DynamicMSTDivAndConqLCT
// Time Complexity:
//   constructor: O(V)
//   addEdge, removeEdge, addMstQuery: O(1)
//   solveQueries: O(V + Q log Q (log Q + log V))
// Memory Complexity: O(V + Q) for Q edge additions/removals and queries
// Tested:
//   https://dmoj.ca/problem/ccoprep4p3
template <class T> struct DynamicMSTDivAndConqUnionFind {
  int V; vector<tuple<int, int, int, T, int>> queries; vector<T> ans;
  DynamicMSTDivAndConqUnionFind(int V) : V(V) {}
  void addEdge(int v, int w, T weight) {
    if (v > w) swap(v, w);
    queries.emplace_back(0, v, w, weight, -1);
  }
  void removeEdge(int v, int w, T weight) {
    if (v > w) swap(v, w);
    queries.emplace_back(1, v, w, weight, -1);
  }
  void addMstQuery() {
    queries.emplace_back(2, -1, -1, T(), -1);
  }
  void solveQueries() {
    vector<tuple<int, int, T>> edges; int Q = queries.size(); edges.reserve(Q);
    for (auto &&q : queries) if (get<0>(q) == 0)
      edges.emplace_back(get<1>(q), get<2>(q), get<3>(q));
    sort(edges.begin(), edges.end());
    vector<int> cntAdd(edges.size(), 0), cntRem = cntAdd;
    for (int i = 0; i < Q; i++) {
      int t, v, w, _; T weight; tie(t, v, w, weight, _) = queries[i];
      tuple<int, int, T> e(v, w, weight); if (t == 0) {
        int j = lower_bound(edges.begin(), edges.end(), e) - edges.begin();
        get<4>(queries[i]) = j + cntAdd[j]++;
      } else if (t == 1) {
        int j = lower_bound(edges.begin(), edges.end(), e) - edges.begin();
        get<4>(queries[i]) = j + cntRem[j]++;
      }
    }
    UnionFindUndo uf(V); ans.clear(); ans.reserve(Q);
    vector<bool> active(edges.size(), false), changed(edges.size(), false);
    auto cmpEdge = [&] (int i, int j) {
      return active[i] == active[j] ? get<2>(edges[i]) < get<2>(edges[j])
                                    : active[i];
    };
    function<void(int, int, vector<int> &, T)> dc
        = [&] (int l, int r, vector<int> &maybe, T curMST) {
      int curSize = uf.history.size();
      if (l == r) {
        int t = get<0>(queries[l]), j = get<4>(queries[l]);
        if (t == 0) active[j] = true;
        else if (t == 1) active[j] = false;
        else if (t == 2) {
          sort(maybe.begin(), maybe.end(), cmpEdge); for (int j : maybe)
            if (active[j] && uf.join(get<0>(edges[j]), get<1>(edges[j])))
              curMST += get<2>(edges[j]);
          ans.push_back(curMST);
          while (int(uf.history.size()) > curSize) uf.undo();
        }
        return;
      }
      sort(maybe.begin(), maybe.end(), cmpEdge);
      for (int i = l; i <= r; i++) if (get<0>(queries[i]) <= 1) {
        int j = get<4>(queries[i]); changed[j] = true;
        uf.join(get<0>(edges[j]), get<1>(edges[j]));
      }
      vector<int> must; for (int j : maybe)
        if (!changed[j] && uf.join(get<0>(edges[j]), get<1>(edges[j])))
          must.push_back(j);
      while (int(uf.history.size()) > curSize) uf.undo();
      for (int j : must) if (uf.join(get<0>(edges[j]), get<1>(edges[j])))
        curMST += get<2>(edges[j]);
      int curSize2 = uf.history.size();
      vector<int> newMaybe; for (int j : maybe)
        if (changed[j] || uf.join(get<0>(edges[j]), get<1>(edges[j])))
          newMaybe.push_back(j);
      while (int(uf.history.size()) > curSize2) uf.undo();
      for (int i = l; i <= r; i++) if (get<0>(queries[i]) <= 1)
        changed[get<4>(queries[i])] = false;
      int m = l + (r - l) / 2;
      dc(l, m, newMaybe, curMST); dc(m + 1, r, newMaybe, curMST);
      while (int(uf.history.size()) > curSize) uf.undo();
    };
    if (Q > 0) {
      vector<int> maybe(edges.size()); iota(maybe.begin(), maybe.end(), 0);
      dc(0, Q - 1, maybe, T());
    }
  }
};
