#pragma once
#include <bits/stdc++.h>
#include "HeavyLightDecomposition.h"
using namespace std;

// Supports path (and subtree) queries on a forest
// Vertices and indices are 0-indexed
// Template Arguments:
//   R: struct supporting range queries on indices
//     Required Fields:
//       Data: the data type
//     Required Functions:
//       static qdef(): returns the query default value of type Data
//       static merge(l, r): merges the datas l and r, must be associative
//       constructor(A): takes a vector A of type Data with the initial
//         value of each index
//       query(l, r, rev): queries the range [l, r] with rev indicating
//         if the result needs to be reversed
//     Sample Struct: supporting range assignment and maximum subarray sum
//       struct R {
//         using Data = MaxSubarraySumCombine<int>::Data;
//         static Data qdef() { return MaxSubarraySumCombine<int>::qdef(); }
//         static Data merge(const Data &l, const Data &r) {
//           return MaxSubarraySumCombine<int>::merge(l, r);
//         }
//         SegmentTreeLazyBottomUp<MaxSubarraySumCombine<int>> ST;
//         R(const vector<Data> &A) : ST(A) {}
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
//   queryPath(v, w): queries the path from v to w
//   querySubtree(v): queries the subtree of vertex v
// In practice, constructor has a moderate constant,
//   query functions have a small constant
// Time Complexity:
//   constructor: O(V) + time complexity of R's constructor
//   queryPath: O(log V) + time complexity of R's query
//   querySubtree: time complexity of R's query
// Memory Complexity: O(V) + memory complexity of R
// Tested:
//   https://dmoj.ca/problem/acc2p3
template <class R, const bool VALUES_ON_EDGES>
struct StaticPathQueries : public HLD {
  using Data = typename R::Data; R ops; vector<Data> down, up;
  Data queryPath(int v, int w) {
    Data qu = R::qdef(), qd = R::qdef(); while (head[v] != head[w]) {
      if (dep[head[v]] < dep[head[w]]) {
        qd = R::merge(down[w], qd); w = par[head[w]];
      } else { qu = R::merge(qu, up[v]); v = par[head[v]]; }
    }
    if (v != w) {
      if (dep[v] < dep[w])
        qu = R::merge(qu, ops.query(pre[v] + VALUES_ON_EDGES, pre[w], false));
      else
        qd = R::merge(ops.query(pre[w] + VALUES_ON_EDGES, pre[v], true), qd);
    } else if (!VALUES_ON_EDGES) {
      int i = pre[dep[v] < dep[w] ? v : w];
      qu = R::merge(qu, ops.query(i, i, false));
    }
    return R::merge(qu, qd);
  }
  Data querySubtree(int v) {
    int l = pre[v] + VALUES_ON_EDGES, r = post[v];
    return l <= r ? ops.query(l, r, false) : R::qdef();
  }
  vector<Data> reorder(const vector<Data> &A) {
    vector<Data> ret; ret.reserve(A.capacity());
    for (int i = 0; i < V; i++) ret.push_back(A[vert[i]]);
    return ret;
  }
  template <class Forest>
  StaticPathQueries(const Forest &G, const vector<Data> &A,
                    const vector<int> &roots = vector<int>())
      : HLD(G, roots), ops(reorder(A)), down(A), up(A) {
    for (int i = 0; i < V; i++) {
      int v = vert[i]; if (head[v] != v) {
        down[v] = R::merge(down[par[v]], A[v]);
        up[v] = R::merge(A[v], up[par[v]]);
      }
    }
  }
  template <class Forest>
  StaticPathQueries(const Forest &G, const vector<Data> &A, int rt)
      : StaticPathQueries(G, A, vector<int>{rt}) {}
};
