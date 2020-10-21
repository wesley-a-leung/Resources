#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/unionfind/UnionFindUndo.h"
using namespace std;

// Support queries on connected components, after edges have been
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
// In practice, has a small constant, faster than DynamicConnectivityLCT
// Time Complexity:
//   constructor: O(1)
//   addEdge, removeEdge, addConnectedQuery, addSizeQuery, addCntQuery: O(1)
//   solveQueries: O(V + Q log Q log V)
// Memory Complexity: O(V + Q) for Q edge additions/removals and queries
// Tested:
//   https://codeforces.com/gym/100551/problem/A
//   https://codeforces.com/gym/100551/problem/E
struct DynamicConnectivityDivAndConq {
  int V; vector<tuple<int, int, int, int>> queries; vector<int> ans;
  DynamicConnectivityDivAndConq(int V) : V(V) {}
  void addEdge(int v, int w) {
    if (v > w) swap(v, w);
    queries.emplace_back(0, v, w, -1);
  }
  void removeEdge(int v, int w) {
    if (v > w) swap(v, w);
    queries.emplace_back(1, v, w, -1);
  }
  void addConnectedQuery(int v, int w) {
    queries.emplace_back(2, v, w, queries.size());
  }
  void addSizeQuery(int v) { queries.emplace_back(3, v, v, queries.size()); }
  void addCntQuery() { queries.emplace_back(4, -1, -1, queries.size()); }
  void solveQueries() {
    vector<pair<int, int>> edges; int Q = queries.size(); edges.reserve(Q);
    for (auto &&q : queries) if (get<0>(q) == 0)
      edges.emplace_back(get<1>(q), get<2>(q));
    sort(edges.begin(), edges.end()); vector<int> last(edges.size(), INT_MAX);
    for (int i = 0; i < Q; i++) {
      int t, v, w, _; tie(t, v, w, _) = queries[i]; if (t == 0) {
        int j = lower_bound(edges.begin(), edges.end(), make_pair(v, w))
            - edges.begin();
        get<3>(queries[i]) = last[j]; last[j] = i;
      } else if (t == 1) {
        int j = lower_bound(edges.begin(), edges.end(), make_pair(v, w))
            - edges.begin();
        int temp = get<3>(queries[get<3>(queries[i]) = last[j]]);
        get<3>(queries[last[j]]) = i; last[j] = temp;
      }
    }
    UnionFindUndo uf(V); ans.clear(); ans.reserve(Q);
    function<void(int, int)> dc = [&] (int l, int r) {
      if (l == r) {
        int t, v, w, _; tie(t, v, w, _) = queries[l];
        if (t == 2) ans.push_back(uf.connected(v, w));
        else if (t == 3) ans.push_back(uf.getSize(v));
        else if (t == 4) ans.push_back(uf.cnt);
        return;
      }
      int m = l + (r - l) / 2, curSize = uf.history.size();
      for (int i = m + 1; i <= r; i++) if (get<3>(queries[i]) < l)
        uf.join(get<1>(queries[i]), get<2>(queries[i]));
      dc(l, m); while (int(uf.history.size()) > curSize) uf.undo();
      for (int i = l; i <= m; i++) if (get<3>(queries[i]) > r)
        uf.join(get<1>(queries[i]), get<2>(queries[i]));
      dc(m + 1, r); while (int(uf.history.size()) > curSize) uf.undo();
    };
    if (Q > 0) dc(0, Q - 1);
  }
};
