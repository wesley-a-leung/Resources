#pragma once
#include <bits/stdc++.h>
#include "../dynamictrees/LinkCutTree.h"
using namespace std;

// Support queries for the number of bridges in a graph, after edges have been
//   added, using a Link Cut Tree
// Constructor Arguments:
//   V: the number of vertices in the graph
// Fields:
//   bridges: the current number of bridges in the graph
// Functions:
//   addEdge(v, w): adds an edge between vertices v and w
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V)
//   addEdge: O(log V)
// Memory Complexity: O(V)
// Tested:
//   https://codeforces.com/gym/100551/problem/B
struct IncrementalBridges {
  struct Node {
    using Data = pair<bool, int>; using Lazy = bool;
    static const bool RANGE_UPDATES = true, RANGE_QUERIES = true;
    static const bool RANGE_REVERSALS = true;
    bool rev; int sz; Node *l, *r, *p; Lazy lz; Data val, sbtr;
    Node(const Data &v)
        : rev(false), sz(1), l(nullptr), r(nullptr), p(nullptr),
          lz(false), val(v), sbtr(v) {}
    void update() {
      sz = 1; sbtr = val;
      if (l) {
        sz += l->sz;
        if (sbtr.first && !l->sbtr.first) sbtr = l->sbtr;
        else if (sbtr.first == l->sbtr.first) sbtr.second += l->sbtr.second;
      }
      if (r) {
        sz += r->sz;
        if (sbtr.first && !r->sbtr.first) sbtr = r->sbtr;
        else if (sbtr.first == r->sbtr.first) sbtr.second += r->sbtr.second;
      }
    }
    void propagate() {
      if (rev) {
        if (l) l->reverse();
        if (r) r->reverse();
        rev = false;
      }
      if (lz) {
        if (l) l->apply(lz);
        if (r) r->apply(lz);
        lz = false;
      }
    }
    void apply(const Lazy &v) {
      lz = v; val.first = v; sbtr.first = v;
    }
    void reverse() { rev = !rev; swap(l, r); }
    static Data qdef() { return make_pair(true, 1); }
  };
  int V, treeEdges, bridges; LCT<Node> lct;
  LCT<Node> init(int V) {
    int i = 0; return LCT<Node>(max(0, V * 2 - 1), [&] {
      return make_pair(i++ < V, 1);
    });
  }
  IncrementalBridges(int V) : V(V), treeEdges(0), bridges(0), lct(init(V)) {}
  void addEdge(int v, int w) {
    if (!lct.connected(v, w)) {
      lct.link(v, V + treeEdges); lct.link(w, V + treeEdges++); bridges++;
      return;
    }
    lct.makeRoot(v); pair<bool, int> q = lct.queryPathFromRoot(w);
    if (!q.first) bridges -= q.second;
    lct.updatePathFromRoot(w, 1);
  }
};
