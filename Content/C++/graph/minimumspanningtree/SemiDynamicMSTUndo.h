#pragma once
#include <bits/stdc++.h>
#include "../dynamictrees/LinkCutTree.h"
using namespace std;

// Supports queries for the minimum spanning tree (or forest) after
//   edges have been added, using a Link Cut Tree, as well as undoing the
//   last edge added
// Template Arguments:
//   T: the type of the weight of the edges
// Constructor Arguments:
//   V: number of vertices in the graph
//   NEG_INF: a value for negative infinity of type T
// Fields:
//   mstWeight: the weight of the current mst
//   mstEdges: a vector of tuples of the edges in the current mst in the form
//     (v, w, weight) representing an undirected edge in the graph between
//     vertices v and w with weight of weight
//   lct: a Link Cut Tree of the current mst of the graph
//   history: a vector of tuples storing information of the last edge added
// Functions:
//   addEdge(v, w, weight): adds an undirected edge in the graph between
//     vertices v and w with weight of weight
//   undo(): undoes the last edge added to the graph
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V)
//   addEdge, undo: O(log V)
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/ccoprep4p3
template <class T> struct SemiDynamicMSTUndo {
  struct Node {
    using Data = pair<T, int>; using Lazy = Data;
    static const bool RANGE_UPDATES = false, RANGE_QUERIES = true;
    static const bool RANGE_REVERSALS = true, HAS_PAR = true;
    bool rev; Node *l, *r, *p; Data val, sbtr;
    Node(const Data &v)
        : rev(false), l(nullptr), r(nullptr), p(nullptr), val(v), sbtr(v) {}
    void update() {
      sbtr = val;
      if (l) { sbtr = max(l->sbtr, sbtr); }
      if (r) { sbtr = max(sbtr, r->sbtr); }
    }
    void propagate() {
      if (rev) {
        swap(l, r); rev = false;
        if (l) l->reverse();
        if (r) r->reverse();
      }
    }
    void apply(const Lazy &v) { val = sbtr = v; }
    void reverse() { rev = !rev; }
    static Data qdef() { return make_pair(T(), -1); }
  };
  using Edge = tuple<int, int, T>; int V, top; vector<int> stk;
  T mstWeight; vector<Edge> mstEdges; LCT<Node> lct;
  vector<tuple<int, Edge, int, Edge, bool>> history;
  SemiDynamicMSTUndo(int V, T NEG_INF = numeric_limits<T>::lowest())
      : V(V), top(max(0, V - 1)), stk(top), mstWeight(T()),
        lct(V + top, [&] { return make_pair(NEG_INF, -1); }) {
    iota(stk.rbegin(), stk.rend(), 0); mstEdges.reserve(top);
  }
  void addEdge(int v, int w, T weight) {
    Edge e(v, w, weight); history.emplace_back(-1, e, -1, e, false);
    if (v == w) return;
    T z; int j; tie(z, j) = lct.queryPath(v, w); if (j != -1) {
      if (z <= weight) return;
      lct.cut(get<0>(mstEdges[j]), V + j); lct.cut(get<1>(mstEdges[j]), V + j);
      get<0>(history.back()) = stk[top++] = j; mstWeight -= z;
      get<1>(history.back()) = mstEdges[j];
    }
    get<2>(history.back()) = j = stk[--top]; if (j >= int(mstEdges.size())) {
      mstEdges.push_back(e); get<4>(history.back()) = true;
    } else mstEdges[j] = e;
    lct.updateVertex(V + j, make_pair(weight, j)); lct.link(v, V + j);
    lct.link(w, V + j); mstWeight += weight; get<3>(history.back()) = e;
  }
  void undo() {
    int j = get<2>(history.back()); Edge e = get<3>(history.back());
    if (j != -1) {
      mstWeight -= get<2>(e); stk[top++] = j;
      lct.cut(get<0>(e), V + j); lct.cut(get<1>(e), V + j);
      if (get<4>(history.back())) mstEdges.pop_back();
    }
    j = get<0>(history.back()); e = get<1>(history.back()); if (j != -1) {
      mstWeight += get<2>(e); --top; mstEdges[j] = e;
      lct.updateVertex(V + j, make_pair(get<2>(e), j));
      lct.link(get<0>(e), V + j); lct.link(get<1>(e), V + j); 
    }
    history.pop_back();
  }
};
