#pragma once
#include <bits/stdc++.h>
#include "HLD.h"
using namespace std;

// Supports path updates and queries on a forest
// Vertices and indices are 0-indexed
// Template Arguments:
//   RangeOps: struct supporting range updates and queries on indices
//     Fields:
//       Data: typedef/using for the data type
//       Lazy: typedef/using for the lazy type
//       qdef: the query default value of type Data
//     Functions:
//       constructor: takes a vector of type Data with the initial value of
//         each index
//       update(l, r, val, rev): updates the range [l, r] with the value
//         val, with rev indicating if the update range should be updated
//         right to left
//       query(l, r, rev): queries the range [l, r] with rev indicating
//         if the result needs to be reversed
//       merge(l, r): merges the datas l and r
//     Sample Struct: supporting range assignment and maximum subarray sum
//       struct RangeOps {
//         using Data = MaxSubarraySumCombine<int>::Data;
//         using Lazy = MaxSubarraySumCombine<int>::Lazy;
//         const Data qdef = MaxSubarraySumCombine<int>().qdef;
//         SegmentTreeLazyBottomUp<MaxSubarraySumCombine<int>> ST;
//         RangeOps(const vector<Data> &A) : ST(A.begin(), A.end()) {}
//         void update(int l, int r, const Lazy &val, bool) {
//           ST.update(l, r, val);
//         }
//         Data query(int l, int r, bool rev) {
//           Data ret = ST.query(l, r);
//           if (rev) swap(ret.pre, ret.suf);
//           return ret;
//         }
//         Data merge(const Data &l, const Data &r) {
//           return MaxSubarraySumCombine<int>().merge(l, r);
//         }
//       };
//   VALUES_ON_EDGES: boolean indicating whether the values are on the edges
//     (the lowest depth vertex of the edge) or the vertices
// Constructor Arguments:
//   G: a generic forest data structure
//     with the [] operator (const) defined to iterate over the adjacency list
//     (which is a list of ints), as well as a member function size() (const)
//     that returns the number of vertices in the forest
//   A: a vector of type RangeOps::Data with the initial value of each vertex
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
//   constructor: O(V) + time complexity of RangeOps constructor
//   updatePath, queryPath: O(log V) * time complexity of update/query
//   updateVertex, updateSubtree, queryPath, querySubtree:
//     time complexity of update/query
// Memory Complexity: O(V) + memory complexity of RangeOps
// Tested:
//   https://www.spoj.com/problems/GSS7/
template <class RangeOps, const bool VALUES_ON_EDGES>
struct PathQueries : public HLD {
  using Data = typename RangeOps::Data; using Lazy = typename RangeOps::Lazy;
  RangeOps R;
  void updatePath(int v, int w, const Lazy &val) {
    while (head[v] != head[w]) {
      if (dep[head[v]] < dep[head[w]]) {
        R.update(pre[head[w]], pre[w], val, false); w = par[head[w]];
      } else { R.update(pre[head[v]], pre[v], val, true); v = par[head[v]]; }
    }
    if (v != w) {
      if (dep[v] < dep[w])
        R.update(pre[v] + VALUES_ON_EDGES, pre[w], val, false);
      else R.update(pre[w] + VALUES_ON_EDGES, pre[v], val, true);
    } else if (!VALUES_ON_EDGES) {
      int i = pre[dep[v] < dep[w] ? v : w]; R.update(i, i, val, true);
    }
  }
  void updateVertex(int v, const Lazy &val) {
    R.update(pre[v], pre[v], val, false);
  }
  void updateSubtree(int v, const Lazy &val) {
    int l = pre[v] + VALUES_ON_EDGES, r = post[v];
    if (l <= r) R.update(l, r, val, false);
  }
  Data queryPath(int v, int w) {
    Data up = R.qdef, down = R.qdef; while (head[v] != head[w]) {
      if (dep[head[v]] < dep[head[w]]) {
        down = R.merge(R.query(pre[head[w]], pre[w], false), down);
        w = par[head[w]];
      } else {
        up = R.merge(up, R.query(pre[head[v]], pre[v], true));
        v = par[head[v]];
      }
    }
    if (v != w) {
      if (dep[v] < dep[w]) 
        up = R.merge(up, R.query(pre[v] + VALUES_ON_EDGES, pre[w], false));
      else
        down = R.merge(R.query(pre[w] + VALUES_ON_EDGES, pre[v], true), down);
    } else if (!VALUES_ON_EDGES) {
      int i = pre[dep[v] < dep[w] ? v : w];
      up = R.merge(up, R.query(i, i, true));
    }
    return R.merge(up, down);
  }
  Data queryVertex(int v) { return R.query(pre[v], pre[v], false); }
  Data querySubtree(int v) {
    int l = pre[v] + VALUES_ON_EDGES, r = post[v];
    return l <= r ? R.query(l, r, false) : R.qdef;
  }
  vector<Data> reorder(const vector<Data> &A) {
    vector<Data> ret; ret.reserve(vert.size());
    for (int i = 0; i < int(vert.size()); i++) ret.push_back(A[vert[i]]);
    return ret;
  }
  template <class Forest> PathQueries(const Forest &G, const vector<Data> &A)
      : HLD(G), R(reorder(A)) {}
};
