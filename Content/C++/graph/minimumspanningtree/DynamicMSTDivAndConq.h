#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFindUndo.h"
using namespace std;

// Support offline queries for the minimum spanning tree, after edges have been
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
// In practice, has a small constant
// Time Complexity:
//   constructor: O(V)
//   addEdge, removeEdge, addMstQuery: O(1)
//   solveQueries: O(V + Q log Q (log Q + log V))
// Memory Complexity: O(V + Q) for Q edge additions/removals and queries
// Tested:
//   https://dmoj.ca/problem/ccoprep4p3
template <class T> struct DynamicMSTDivAndConq {
  int V; vector<tuple<int, int, int, T, int>> queries; vector<T> ans;
  DynamicMSTDivAndConq(int V) : V(V) {}
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
    vector<int> cntAdd(edges.size(), 0), cntRem = cntAdd, qinds{-1};
    qinds.reserve(Q); for (int i = 0; i < Q; i++) {
      int t, v, w, _; T weight; tie(t, v, w, weight, _) = queries[i];
      tuple<int, int, T> e(v, w, weight); if (t == 0) {
        int j = lower_bound(edges.begin(), edges.end(), e) - edges.begin();
        get<4>(queries[i]) = j + cntAdd[j]++;
      } else if (t == 1) {
        int j = lower_bound(edges.begin(), edges.end(), e) - edges.begin();
        get<4>(queries[i]) = j + cntRem[j]++;
      } else if (t == 2) qinds.push_back(i);
    }
    UnionFindUndo uf(V); ans.clear(); ans.reserve(Q);
    vector<bool> active(edges.size(), false), changed(edges.size(), false);
    auto cmpEdge = [&] (int i, int j) {
      return active[i] == active[j] ? get<2>(edges[i]) < get<2>(edges[j])
                                    : active[i];
    };
    function<void(int, int, vector<int> &, T)> dc
        = [&] (int ql, int qr, vector<int> &maybe, T curMST) {
      int curSize = uf.history.size(); if (ql == qr) {
        for (int i = qinds[ql - 1] + 1; i <= qinds[qr]; i++) {
          int t = get<0>(queries[i]), j = get<4>(queries[i]);
          if (t == 0) active[j] = true;
          else if (t == 1) active[j] = false;
          else if (t == 2) {
            sort(maybe.begin(), maybe.end(), cmpEdge); for (int j : maybe)
              if (active[j] && uf.join(get<0>(edges[j]), get<1>(edges[j])))
                curMST += get<2>(edges[j]);
            ans.push_back(curMST);
            while (int(uf.history.size()) > curSize) uf.undo();
          }
        }
        return;
      }
      sort(maybe.begin(), maybe.end(), cmpEdge);
      for (int i = qinds[ql - 1] + 1; i <= qinds[qr]; i++)
        if (get<0>(queries[i]) <= 1) {
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
      for (int i = qinds[ql - 1] + 1; i <= qinds[qr]; i++)
        if (get<0>(queries[i]) <= 1) changed[get<4>(queries[i])] = false;
      int qm = ql + (qr - ql) / 2;
      dc(ql, qm, newMaybe, curMST); dc(qm + 1, qr, newMaybe, curMST);
      while (int(uf.history.size()) > curSize) uf.undo();
    };
    if (int(qinds.size()) > 1) {
      vector<int> maybe(edges.size()); iota(maybe.begin(), maybe.end(), 0);
      dc(1, int(qinds.size()) - 1, maybe, T());
    }
  }
};
