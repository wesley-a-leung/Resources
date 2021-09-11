#pragma once
#include <bits/stdc++.h>
#include "../dynamictrees/LinkCutTree.h"
using namespace std;

// Support queries for the number of bridges in a graph, after edges have been
//   added, using a Link Cut Tree, as well as undoing the last edge added
// Constructor Arguments:
//   V: the number of vertices in the graph
// Fields:
//   bridgeCnt: the current number of bridges in the graph
//   history: a vector of tuples storing information of the last edge added
// Functions:
//   addEdge(v, w): adds an edge between vertices v and w
//   undo(v, w): undoes the last edge added to the graph
//   twoEdgeConnected(v, w): queries whether v and w are in the
//     same 2-edge connected component
// Time Complexity:
//   constructor: O(V)
//   addEdge, undo, twoEdgeConnected: O(log V) amortized
// Memory Complexity: O(V)
// Tested:
//   https://codeforces.com/gym/100551/problem/D
struct IncrementalBridgesUndo {
  struct Node {
    using Data = pair<int, int>; using Lazy = int;
    static const bool RANGE_UPDATES = true, RANGE_QUERIES = true;
    static const bool RANGE_REVERSALS = true;
    bool rev; int sz; Node *l, *r, *p; Lazy lz; Data val, sbtr;
    Node(const Data &v)
        : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr),
          lz(0), val(v), sbtr(v) {}
    void update() {
      sz = 1; sbtr = val;
      if (l) {
        sz += l->sz;
        if (sbtr.first < l->sbtr.first) sbtr = l->sbtr;
        else if (sbtr.first == l->sbtr.first) sbtr.second += l->sbtr.second;
      }
      if (r) {
        sz += r->sz;
        if (sbtr.first < r->sbtr.first) sbtr = r->sbtr;
        else if (sbtr.first == r->sbtr.first) sbtr.second += r->sbtr.second;
      }
    }
    void propagate() {
      if (rev) {
        if (l) l->reverse();
        if (r) r->reverse();
        rev = false;
      }
      if (lz != 0) {
        if (l) l->apply(lz);
        if (r) r->apply(lz);
        lz = 0;
      }
    }
    void apply(const Lazy &v) {
      lz += v; val.first += v; sbtr.first += v;
    }
    void reverse() { rev = !rev; swap(l, r); }
    static Data qdef() { return make_pair(0, 0); }
  };
  int V, treeEdges, bridgeCnt; LCT<Node> lct;
  vector<tuple<int, int, int>> history;
  vector<pair<int, int>> init(int V) {
    vector<pair<int, int>> ret(max(0, V * 2 - 1), make_pair(1, 1));
    fill(ret.begin(), ret.begin() + V, make_pair(0, 1)); return ret;
  }
  IncrementalBridgesUndo(int V)
      : V(V), treeEdges(0), bridgeCnt(0), lct(init(V)) {}
  void addEdge(int v, int w) {
    pair<int, int> q = lct.queryPath(v, w); if (q.second == 0) {
      lct.link(v, V + treeEdges); lct.link(w, V + treeEdges++); bridgeCnt++;
      history.emplace_back(v, w, -1); return;
    }
    lct.updatePathFromRoot(w, -1); if (q.first == 1) bridgeCnt -= q.second;
    history.emplace_back(v, w, q.first == 1 ? q.second : 0);
  }
  void undo() {
    int v, w, delta; tie(v, w, delta) = history.back(); history.pop_back();
    if (delta < 0) {
      lct.cut(v, V + --treeEdges); lct.cut(w, V + treeEdges); bridgeCnt--;
      return;
    }
    lct.updatePath(v, w, 1); bridgeCnt += delta;
  }
  bool twoEdgeConnected(int v, int w) {
    return lct.queryPath(v, w).first == 0;
  }
};
