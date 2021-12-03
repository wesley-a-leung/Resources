#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/BitPrefixSumArray.h"
#include "LowestCommonAncestor.h"
using namespace std;

// Wavelet Matrix supporting 2D aggregation path queries where the data can
//   change, but not the keys
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the element of the array
//   R: struct supporting point updates and prefix queries on indices
//     Required Fields:
//       Data: the data type
//       Lazy: the lazy type
//     Required Functions:
//       static qdef(): returns the query default value
//       static merge(l, r): returns the values l of type Data merged with
//         r of type Data, must be associative and commutative
//       static invData(v): returns the inverse of v of type Data
//       constructor(A): takes a vector A of type Data with the initial
//         value of each index
//       update(i, val): updates the index i with the value val
//       query(r): queries the prefix ending at index r
//     Sample Struct: supporting point increments updates and
//         prefix sum queries
//       struct R {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         static Data invData(const Data &v) { return -v; }
//         FenwickTree1D<Data> FT;
//         R(const vector<Data> &A) : FT(A) {}
//         void update(int i, const Lazy &val) { FT.update(i, val); }
//         Data query(int r) { return FT.query(r); }
//       };
//   VALUES_ON_EDGES: boolean indicating whether the values are on the edges
//     (the largest depth vertex of the edge) or the vertices
//   Cmp: the comparator to compare two elements
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   G: a generic forest data structure
//     Required Functions:
//       operator [v] const: iterates over the adjacency list of vertex v
//         (which is a list of ints)
//       size() const: returns the number of vertices in the forest
//   A: a vector of type T of the values in the array
//   X: a vector of type R::Data containing the initial data of the array
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
//   cmp: an instance of the Cmp struct
// Functions:
//   update(v, val): updates the vertex v with the lazy value v
//   query(v, w, hi): returns the aggregate value of the data associated with
//     all elements with a key less than or equal to hi (using the comparator)
//     on the path from v to w
//   query(v, w, lo, hi): returns the aggregate value of the data associated
//     with all elements with a key of not less than lo and not greater than hi
//     (using the comparator) on the path from v to w
//   bsearch(v, w, f): over all keys in the array, finds the first key k such
//     that query(v, w, k) returns true
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O((V + C) log V) where C is the time complexity of
//     R's constructor
//   update: O(U log V) where U is the time complexity of R's update function
//   query, bsearch: O(Q log V) where Q is the time complexity of
//     R's query function
// Memory Complexity: O(V + (V log V) / 64 + M log V) where M is the memory
//   complexity of R
// Tested:
//   Fuzz and Stress Tested
template <class T, class R, const bool VALUES_ON_EDGES, class Cmp = less<T>>
struct WaveletMatrixTreeAggregation {
#define clt [&] (const T &a, const T &b) { return cmp(a, b); }
#define cle [&] (const T &a, const T &b) { return !cmp(b, a); }
  using Data = typename R::Data; using Lazy = typename R::Lazy;
  int V, H, preInd, postInd; vector<int> par, pre, post, last, mid1, mid2;
  vector<T> S; LCA<> lca; vector<BitPrefixSumArray> B1, B2; vector<R> D1, D2;
  Cmp cmp;
  template <class Forest> void dfs(const Forest &G, int v, int prev) {
    par[v] = prev; last[pre[v] = preInd++] = postInd - 1;
    for (int w : G[v]) if (w != prev) dfs(G, w, v);
    post[v] = postInd++;
  }
  void build(const vector<int> &A, const vector<Data> &X,
             const vector<int> &ind, vector<int> &mid,
             vector<BitPrefixSumArray> &B, vector<R> &D) {
    vector<int> C(V), P(V); vector<Data> Y = X, Z = X; D.reserve(H);
    for (int v = 0; v < V; v++) { C[ind[v]] = A[v]; Y[ind[v]] = X[v]; }
    iota(P.begin(), P.end(), 0); for (int h = H - 1; h >= 0; h--) {
      int ph = 1 << h;
      for (int i = 0; i < V; i++) B[h].set(i, C[P[i]] <= ph - 1); 
      mid[h] = stable_partition(P.begin(), P.end(), [&] (int i) {
                                  return C[i] <= ph - 1;
                                }) - P.begin();
      B[h].build(); for (int i = 0; i < V; i++) Z[i] = Y[P[i]];
      D.emplace_back(Z); for (int i = mid[h]; i < V; i++) C[P[i]] -= ph;
    }
    reverse(D.begin(), D.end());
  }
  template <class Forest>
  WaveletMatrixTreeAggregation(
      const Forest &G, const vector<T> &A, const vector<Data> &X,
      const vector<int> &roots = vector<int>(), Cmp cmp = Cmp())
      : V(A.size()), H(V == 0 ? 0 : __lg(V) + 1), preInd(0), postInd(0),
        par(V, -1), pre(V), post(V), last(V), mid1(H), mid2(H),
        S(A), lca(G, roots), B1(H, BitPrefixSumArray(V)), B2(B1), cmp(cmp) {
    sort(S.begin(), S.end(), cmp); if (roots.empty()) {
      for (int v = 0; v < V; v++) if (par[v] == -1) dfs(G, v, -1);
    } else for (int v : roots) dfs(G, v, -1);
    vector<int> C(V); for (int v = 0; v < V; v++)
      C[v] = lower_bound(S.begin(), S.end(), A[v], cmp) - S.begin();
    build(C, X, pre, mid1, B1, D1); build(C, X, post, mid2, B2, D2);
  }
  template <class Forest>
  WaveletMatrixTreeAggregation(const Forest &G, const vector<T> &A,
                               const vector<Data> &X, int rt, Cmp cmp = Cmp())
      : WaveletMatrixTreeAggregation(G, A, X, vector<int>{rt}, cmp) {}
  bool connected(int v, int w) { return lca.connected(v, w); }
  using Ranges = vector<tuple<int, int, int, int, int>>;
  Ranges getRanges(int v, int w) {
    int u = lca.lca(v, w), rt = lca.root[v]; Ranges ranges; ranges.reserve(5);
    int t = VALUES_ON_EDGES ? 1 : (par[u] != -1 ? 2 : 0);
    if (t && pre[rt] - 1 > 0)
      ranges.emplace_back(pre[rt] - 1, last[pre[rt] - 1], -1, 0, 0);
    ranges.emplace_back(pre[v], last[pre[v]], 1, 0, 0);
    ranges.emplace_back(pre[w], last[pre[w]], 1, 0, 0);
    ranges.emplace_back(pre[u], last[pre[u]], t == 1 ? -2 : -1, 0, 0);
    if (t == 2) ranges.emplace_back(pre[par[u]], last[pre[par[u]]], -1, 0, 0);
    return ranges;
  }
  void update(int v, const Lazy &val) {
    for (int a = pre[v], b = post[v], h = H - 1; h >= 0; h--) {
      if (B1[h].get(a)) a = B1[h].query(a - 1);
      else a += mid1[h] - B1[h].query(a - 1);
      if (B2[h].get(b)) b = B2[h].query(b - 1);
      else b += mid2[h] - B2[h].query(b - 1);
      D1[h].update(a, val); D2[h].update(b, val);
    }
  }
  void qryRanges(int h, Ranges &ranges) {
    for (auto &&r : ranges) {
      get<3>(r) = B1[h].query(get<0>(r)); get<4>(r) = B2[h].query(get<1>(r));
    }
  }
  Data qryAgg(int h, const Ranges &ranges) {
    Data ret = R::qdef(); for (auto &&r : ranges) {
      int a = get<3>(r) - 1, b = get<4>(r) - 1, t = get<2>(r);
      Data q = a >= 0 ? D1[h].query(a) : R::qdef();
      if (b >= 0) q = R::merge(q, R::invData(D2[h].query(b)));
      if (t <= -1) q = R::invData(q);
      if (t == -2) q = R::merge(q, q);
      ret = R::merge(ret, q);
    }
    return ret;
  }
  void left(Ranges &ranges) {
    for (auto &&r : ranges) {
      get<0>(r) = get<3>(r) - 1; get<1>(r) = get<4>(r) - 1;
    }
  }
  void right(int h, Ranges &ranges) {
    for (auto &&r : ranges) {
      get<0>(r) += mid1[h] - get<3>(r); get<1>(r) += mid2[h] - get<4>(r);
    }
  }
  template <class F> Data qry(int v, int w, const T &x, F f) {
    Ranges ranges = getRanges(v, w); Data ret = R::qdef();
    for (int cur = 0, h = H - 1; h >= 0; h--) {
      int ph = 1 << h; qryRanges(h, ranges);
      if (cur + ph - 1 >= V || f(x, S[cur + ph - 1])) left(ranges);
      else {
        ret = R::merge(ret, qryAgg(h, ranges)); cur += ph; right(h, ranges);
      }
    }
    return ret;
  }
  Data query(int v, int w, const T &hi) { return qry(v, w, hi, clt); }
  Data query(int v, int w, const T &lo, const T &hi) {
    return R::merge(qry(v, w, hi, clt), R::invData(qry(v, w, lo, cle)));
  }
  template <class F> pair<bool, T *> bsearch(int v, int w, F f) {
    Ranges ranges = getRanges(v, w);
    int cur = 0; Data agg = R::qdef(); for (int h = H - 1; h >= 0; h--) {
      qryRanges(h, ranges); Data val = qryAgg(h, ranges);
      if (f(R::merge(agg, val))) left(ranges);
      else { cur += 1 << h; agg = R::merge(agg, val); right(h, ranges); }
    }
    return make_pair(cur < V, cur < V ? &S[cur] : nullptr);
  }
#undef clt
#undef cle
};
