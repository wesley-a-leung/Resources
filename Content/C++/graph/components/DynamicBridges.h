#pragma once
#include <bits/stdc++.h>
#include "../dynamictrees/LinkCutTree.h"
using namespace std;

// Support offline queries for the number of bridges in a graph, after edges
//   have been added or removed, using a Link Cut Tree
// Constructor Arguments:
//   V: the number of vertices in the graph
// Fields:
//   ans: a vector of integers with the answer for each query
// Functions:
//   addEdge(v, w): adds an edge between vertices v and w
//   removeEdge(v, w): removes an edge between vertices v and w, assuming
//     an edge exists
//   addBridgeQuery(): adds a query for the number of bridges in the graph
//   solveQueries(): solves all queries asked so far
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(1)
//   addEdge, removeEdge, addBridgeQuery: O(1)
//   solveQueries: O(V + Q (log Q + log V))
// Memory Complexity: O(V + Q) for Q edge additions/removals and queries
// Tested:
//   https://codeforces.com/gym/100551/problem/D
struct DynamicBridges {
  struct Node {
    static constexpr const int NO_COVER = INT_MIN, NO_DEL = INT_MAX;
    static void check(int &a, const int &b) {
      if ((a > b && b != NO_COVER) || a == NO_COVER) a = b;
    }
    using Data = pair<int, int>; using Lazy = Data;
    static const bool RANGE_UPDATES = false, RANGE_QUERIES = true;
    static const bool RANGE_REVERSALS = true;
    bool isEdge, rev; Data val, sbtr;
    int edgeCnt, coveredCntSub, coverLazy, covered, coveredSub;
    Node *l, *r, *p;
    Node(const Data &v)
        : isEdge(false), rev(false), val(v), sbtr(v), edgeCnt(0),
          coveredCntSub(0), coverLazy(NO_COVER), covered(NO_COVER),
          coveredSub(NO_COVER), l(nullptr), r(nullptr), p(nullptr) {}
    int getCoveredCnt() {
      return coverLazy == NO_COVER ? coveredCntSub : edgeCnt;
    }
    void update() {
      edgeCnt = isEdge; coveredCntSub = isEdge && (covered != NO_COVER);
      coveredSub = covered; sbtr = val;
      if (l) {
        check(coveredSub, l->coveredSub); check(coveredSub, l->coverLazy);
        edgeCnt += l->edgeCnt; coveredCntSub += l->getCoveredCnt();
        sbtr = min(l->sbtr, sbtr);
      }
      if (r) {
        check(coveredSub, r->coveredSub); check(coveredSub, r->coverLazy);
        edgeCnt += r->edgeCnt; coveredCntSub += r->getCoveredCnt();
        sbtr = min(r->sbtr, sbtr);
      }
    }
    void propagate() {
      if (rev) {
        if (l) l->reverse();
        if (r) r->reverse();
        rev = false;
      }
      if (coverLazy != NO_COVER) {
        covered = max(covered, coverLazy); check(coveredSub, coverLazy);
        if (l) l->coverLazy = max(l->coverLazy, coverLazy);
        if (r) r->coverLazy = max(r->coverLazy, coverLazy);
        coveredCntSub = edgeCnt; coverLazy = NO_COVER;
      }
    }
    void removeCover(int cover) {
      if (coverLazy <= cover) coverLazy = NO_COVER;
      if (coveredSub == NO_COVER || coveredSub > cover) return;
      if (covered <= cover) covered = NO_COVER;
      if (l) l->removeCover(cover);
      if (r) r->removeCover(cover);
      propagate(); update();
    }
    void reverse() { rev = !rev; swap(l, r); }
    static Data qdef() { return make_pair(NO_DEL, -1); }
  };
  int V; vector<tuple<int, int, int, int>> queries; vector<int> ans;
  DynamicBridges(int V) : V(V) {}
  void addEdge(int v, int w) {
    if (v > w) swap(v, w);
    queries.emplace_back(0, v, w, -1);
  }
  void removeEdge(int v, int w) {
    if (v > w) swap(v, w);
    queries.emplace_back(1, v, w, -1);
  }
  void addBridgeQuery() { queries.emplace_back(2, -1, -1, -1); }
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
    vector<pair<int, int>> tmp(V + Q, make_pair(Node::NO_DEL, -1));
    for (int i = 0; i < Q; i++) tmp[V + i] = make_pair(get<3>(queries[i]), i);
    LCT<Node> lct(tmp); int bridges = 0; for (int i = 0; i < Q; i++)
       lct.TR[V + i].edgeCnt = int(lct.TR[V + i].isEdge = true);
    auto cover = [&] (int x, int y, int coverId) {
      lct.queryPath(x, y); bridges += lct.TR[y].getCoveredCnt();
      lct.TR[y].coverLazy = coverId;
      bridges -= lct.TR[y].getCoveredCnt();
    };
    auto uncover = [&] (int x, int y, int coverId) {
      lct.queryPath(x, y); bridges += lct.TR[y].getCoveredCnt();
      lct.TR[y].removeCover(coverId);
      bridges -= lct.TR[y].getCoveredCnt();
    };
    auto addTreeEdge = [&] (int v, int w, int i) {
      lct.link(v, V + i); lct.link(w, V + i); bridges++;
    };
    auto removeTreeEdge = [&] (int v, int w, int i) {
      lct.cut(v, V + i); lct.cut(w, V + i);
      bridges += lct.TR[V + i].getCoveredCnt() - 1;
    };
    ans.clear(); for (int i = 0; i < Q; i++) {
      int t, v, w, o; tie(t, v, w, o) = queries[i]; if (t == 0) {
        if (v == w) continue;
        int z, j; tie(z, j) = lct.queryPath(v, w);
        if (j == -1) addTreeEdge(v, w, i);
        else {
          if (z >= o) { cover(v, w, o); continue; }
          int x = get<1>(queries[j]), y = get<2>(queries[j]);
          removeTreeEdge(x, y, j); addTreeEdge(v, w, i); cover(x, y, z);
        }
      } else if (t == 1) {
        if (v == w) continue;
        if (lct.connected(v, V + o)) removeTreeEdge(v, w, o);
        else uncover(v, w, i);
      } else if (t == 2) ans.push_back(bridges);
    }
  }
};
