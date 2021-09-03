#pragma once
#include <bits/stdc++.h>
#include "../dynamictrees/LinkCutTree.h"
using namespace std;

// Support offline queries on connected components, after edges have been
//   added or removed, using a Link Cut Tree
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
//   addCntQuery(): adds a query asking for the number of connected components
//   solveQueries(): solves all queries asked so far
// In practice, has a moderate constant, slower than
//   DynamicConnectivityDivAndConq, faster than
//   DynamicConnectivityLevelStructure
// Time Complexity:
//   constructor: O(1)
//   addEdge, removeEdge, addConnectedQuery, addCntQuery: O(1)
//   solveQueries: O(V + Q (log Q + log V))
// Memory Complexity: O(V + Q) for Q edge additions/removals and queries
// Tested:
//   https://codeforces.com/gym/100551/problem/A
//   https://codeforces.com/gym/100551/problem/E
struct DynamicConnectivityLCT {
  struct Node {
    using Data = pair<int, int>; using Lazy = Data;
    static const bool RANGE_UPDATES = false, RANGE_QUERIES = true;
    static const bool RANGE_REVERSALS = true, HAS_PAR = true;
    bool rev; Node *l, *r, *p; Data val, sbtr;
    Node(const Data &v)
        : rev(false), l(nullptr), r(nullptr), p(nullptr), val(v), sbtr(v) {}
    void update() {
      sbtr = val;
      if (l) { sbtr = min(l->sbtr, sbtr); }
      if (r) { sbtr = min(sbtr, r->sbtr); }
    }
    void propagate() {
      if (rev) {
        swap(l, r); rev = false;
        if (l) l->reverse();
        if (r) r->reverse();
      }
    }
    void reverse() { rev = !rev; }
    static Data qdef() { return make_pair(INT_MAX, -1); }
  };
  int V; vector<tuple<int, int, int, int>> queries; vector<int> ans;
  DynamicConnectivityLCT(int V) : V(V) {}
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
  void addCntQuery() { queries.emplace_back(3, -1, -1, queries.size()); }
  void solveQueries() {
    vector<pair<int, int>> edges; int Q = queries.size(); edges.reserve(Q);
    for (auto &&q : queries) if (get<0>(q) == 0)
      edges.emplace_back(get<1>(q), get<2>(q));
    sort(edges.begin(), edges.end()); vector<int> last(edges.size(), Q);
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
    vector<pair<int, int>> tmp(V + Q, make_pair(INT_MAX, -1));
    for (int i = 0; i < Q; i++) tmp[V + i] = make_pair(get<3>(queries[i]), i);
    LCT<Node> lct(tmp); ans.clear(); for (int i = 0, cnt = V; i < Q; i++) {
      int t, v, w, o; tie(t, v, w, o) = queries[i]; if (t == 0) {
        if (v == w) continue;
        int z, j; tie(z, j) = lct.queryPath(v, w); if (j != -1) {
          if (z >= o) continue;
          lct.cut(get<1>(queries[j]), V + j);
          lct.cut(get<2>(queries[j]), V + j); cnt++;
        }
        lct.link(v, V + i); lct.link(w, V + i); cnt--;
      } else if (t == 1) {
        if (v == w) continue;
        if (lct.connected(v, V + o)) {
          lct.cut(v, V + o); lct.cut(w, V + o); cnt++;
        }
      } else if (t == 2) ans.push_back(lct.connected(v, w));
      else if (t == 3) ans.push_back(cnt);
    }
  }
};
