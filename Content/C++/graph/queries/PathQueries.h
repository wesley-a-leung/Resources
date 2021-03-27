#pragma once
#include <bits/stdc++.h>
#include "HeavyLightDecomposition.h"
using namespace std;

// Supports path (and subtree) updates and queries on a forest
// Vertices and indices are 0-indexed
// Template Arguments:
//   R: struct supporting range updates and queries on indices
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static merge(l, r): merges the datas l and r, must be associative
//       constructor(A): takes a vector A of type Data with the initial
//         value of each index
//       update(l, r, val, rev): updates the range [l, r] with the value
//         val, with rev indicating if the update range should be updated
//         right to left
//       query(l, r, rev): queries the range [l, r] with rev indicating
//         if the result needs to be reversed
//     Sample Struct: supporting range assignment and maximum subarray sum
//       struct R {
//         using Data = MaxSubarraySumCombine<int>::Data;
//         using Lazy = MaxSubarraySumCombine<int>::Lazy;
//         static Data qdef() { return MaxSubarraySumCombine<int>::qdef(); }
//         static Data merge(const Data &l, const Data &r) {
//           return MaxSubarraySumCombine<int>::merge(l, r);
//         }
//         SegmentTreeLazyBottomUp<MaxSubarraySumCombine<int>> ST;
//         R(const vector<Data> &A) : ST(A.begin(), A.end()) {}
//         void update(int l, int r, const Lazy &val, bool) {
//           ST.update(l, r, val);
//         }
//         Data query(int l, int r, bool rev) {
//           Data ret = ST.query(l, r);
//           if (rev) swap(ret.pre, ret.suf);
//           return ret;
//         }
//       };
//   VALUES_ON_EDGES: boolean indicating whether the values are on the edges
//     (the largest depth vertex of the edge) or the vertices
// Constructor Arguments:
//   G: a generic forest data structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
//   A: a vector of type R::Data with the initial value of each vertex
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
// Functions:
//   updatePath(v, w, val): updates the path from v to w with the value val
//   updateVertex(v, val): updates the vertex v with the value val
//   updateSubtree(v, val): updates the subtree of vertex v with the value val
//   queryPath(v, w): queries the path from v to w
//   queryVertex(v): queries the vertex v
//   querySubtree(v): queries the subtree of vertex v
// In practice, constructor has a moderate constant,
//   update and query functions have a small constant
// Time Complexity:
//   constructor: O(V) + time complexity of R's constructor
//   updatePath, queryPath: O(log V) * time complexity of update/query
//   updateVertex, updateSubtree, queryPath, querySubtree:
//     time complexity of update/query
// Memory Complexity: O(V) + memory complexity of R
// Tested:
//   https://www.spoj.com/problems/GSS7/
//   https://judge.yosupo.jp/problem/vertex_set_path_composite
template <class R, const bool VALUES_ON_EDGES>
struct PathQueries : public HLD {
  using Data = typename R::Data; using Lazy = typename R::Lazy; R ops;
  void updatePath(int v, int w, const Lazy &val) {
    while (head[v] != head[w]) {
      if (dep[head[v]] < dep[head[w]]) {
        ops.update(pre[head[w]], pre[w], val, false); w = par[head[w]];
      } else { ops.update(pre[head[v]], pre[v], val, true); v = par[head[v]]; }
    }
    if (v != w) {
      if (dep[v] < dep[w])
        ops.update(pre[v] + VALUES_ON_EDGES, pre[w], val, false);
      else ops.update(pre[w] + VALUES_ON_EDGES, pre[v], val, true);
    } else if (!VALUES_ON_EDGES) {
      int i = pre[dep[v] < dep[w] ? v : w]; ops.update(i, i, val, true);
    }
  }
  void updateVertex(int v, const Lazy &val) {
    ops.update(pre[v], pre[v], val, false);
  }
  void updateSubtree(int v, const Lazy &val) {
    int l = pre[v] + VALUES_ON_EDGES, r = post[v];
    if (l <= r) ops.update(l, r, val, false);
  }
  Data queryPath(int v, int w) {
    Data qu = R::qdef(), qd = R::qdef(); while (head[v] != head[w]) {
      if (dep[head[v]] < dep[head[w]]) {
        qd = R::merge(ops.query(pre[head[w]], pre[w], false), qd);
        w = par[head[w]];
      } else {
        qu = R::merge(qu, ops.query(pre[head[v]], pre[v], true));
        v = par[head[v]];
      }
    }
    if (v != w) {
      if (dep[v] < dep[w]) 
        qu = R::merge(qu, ops.query(pre[v] + VALUES_ON_EDGES, pre[w], false));
      else
        qd = R::merge(ops.query(pre[w] + VALUES_ON_EDGES, pre[v], true), qd);
    } else if (!VALUES_ON_EDGES) {
      int i = pre[dep[v] < dep[w] ? v : w];
      qu = R::merge(qu, ops.query(i, i, true));
    }
    return R::merge(qu, qd);
  }
  Data queryVertex(int v) { return ops.query(pre[v], pre[v], false); }
  Data querySubtree(int v) {
    int l = pre[v] + VALUES_ON_EDGES, r = post[v];
    return l <= r ? ops.query(l, r, false) : ops.qdef;
  }
  vector<Data> reorder(const vector<Data> &A) {
    vector<Data> ret; ret.reserve(V);
    for (int i = 0; i < V; i++) ret.push_back(A[vert[i]]);
    return ret;
  }
  template <class Forest>
  PathQueries(const Forest &G, const vector<Data> &A,
              const vector<int> &roots = vector<int>())
      : HLD(G, roots), ops(reorder(A)) {}
  template <class Forest>
  PathQueries(const Forest &G, const vector<Data> &A, int rt)
      : HLD(G, rt), ops(reorder(A)) {}
};
