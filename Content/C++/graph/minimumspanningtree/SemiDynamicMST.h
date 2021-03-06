#pragma once
#include <bits/stdc++.h>
#include "../dynamictrees/LinkCutTree.h"
using namespace std;

// Supports queries for the minimum spanning tree (or forest) after
//   edges have been added, using a Link Cut Tree
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
// Functions:
//   addEdge(v, w, weight): adds an undirected edge in the graph between
//     vertices v and w with weight of weight
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V)
//   addEdge: O(log V)
// Memory Complexity: O(V)
// Tested:
//   https://dmoj.ca/problem/noi14p2
//   https://open.kattis.com/problems/minspantree
//   https://codeforces.com/gym/101047/problem/I
template <class T> struct SemiDynamicMST {
  struct Node {
    using Data = pair<T, int>; using Lazy = Data;
    static const bool RANGE_UPDATES = false, RANGE_QUERIES = true;
    static const bool RANGE_REVERSALS = true;
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
  SemiDynamicMST(int V, T NEG_INF = numeric_limits<T>::lowest())
      : V(V), top(max(0, V - 1)), stk(top), mstWeight(T()),
        lct(V + top, [&] { return make_pair(NEG_INF, -1); }) {
    iota(stk.rbegin(), stk.rend(), 0); mstEdges.reserve(top);
  }
  void addEdge(int v, int w, T weight) {
    if (v == w) return;
    T z; int j; tie(z, j) = lct.queryPath(v, w); if (j != -1) {
      if (z <= weight) return;
      lct.cut(get<0>(mstEdges[j]), V + j); lct.cut(get<1>(mstEdges[j]), V + j);
      stk[top++] = j; mstWeight -= z;
    }
    j = stk[--top]; Edge e(v, w, weight);
    if (j >= int(mstEdges.size())) mstEdges.push_back(e);
    else mstEdges[j] = e;
    lct.updateVertex(V + j, make_pair(weight, j));
    lct.link(v, V + j); lct.link(w, V + j); mstWeight += weight;
  }
};
