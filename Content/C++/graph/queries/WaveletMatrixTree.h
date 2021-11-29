#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/BitPrefixSumArray.h"
#include "LowestCommonAncestor.h"
using namespace std;

// Wavelet Matrix supporting rank and select operations for paths on a forest
// Vertices are 0-indexed
// Template Arguments:
//   T: the type of the element of the array
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
//   rt: a single root vertex
//   roots: a vector of root vertices for each connected component
//   cmp: an instance of the Cmp struct
// Functions:
//   rank(v, w, x): returns the number of values less than k (using the
//     comparator) on the path from v to w
//   count(v, w, lo, hi) returns the number of values not less than lo and
//     not greater than hi (using the comparator) on the path from v to w
//   select(v, w, k): returns the kth element (0-indexed) sorted by the
//     comparator if the values on the the path from v to w were sorted
// In practice, has a moderate constant
// Time Complexity:
//   constructor: O(V log V)
//   rank, count, select: O(log V)
// Memory Complexity: O(V + (V log V) / 64)
// Tested:
//   https://mcpt.ca/problem/hld2 (rank/count)
//   https://www.acmicpc.net/problem/11932 (select)
template <class T, const bool VALUES_ON_EDGES, class Cmp = less<T>>
struct WaveletMatrixTree {
#define clt [&] (const T &a, const T &b) { return cmp(a, b); }
#define cle [&] (const T &a, const T &b) { return !cmp(b, a); }
  int V, H, preInd, postInd; vector<int> par, pre, post, last, mid1, mid2;
  vector<T> S; LCA<> lca; vector<BitPrefixSumArray> B1, B2; Cmp cmp;
  template <class Forest> void dfs(const Forest &G, int v, int prev) {
    par[v] = prev; last[pre[v] = preInd++] = postInd - 1;
    for (int w : G[v]) if (w != prev) dfs(G, w, v);
    post[v] = postInd++;
  }
  void build(const vector<int> &A, const vector<int> &ind, vector<int> &mid,
             vector<BitPrefixSumArray> &B) {
    vector<int> C(V); for (int v = 0; v < V; v++) C[ind[v]] = A[v];
    for (int h = H - 1; h >= 0; h--) {
      int ph = 1 << h; for (int i = 0; i < V; i++) B[h].set(i, C[i] <= ph - 1);
      mid[h] = stable_partition(C.begin(), C.end(), [&] (int v) {
                                  return v <= ph - 1;
                                }) - C.begin();
      B[h].build(); for (int i = mid[h]; i < V; i++) C[i] -= ph;
    }
  }
  template <class Forest>
  WaveletMatrixTree(const Forest &G, const vector<T> &A,
                    const vector<int> &roots = vector<int>(), Cmp cmp = Cmp())
      : V(A.size()), H(V == 0 ? 0 : __lg(V) + 1), preInd(0), postInd(0),
        par(V, -1), pre(V), post(V), last(V), mid1(H), mid2(H),
        S(A), lca(G, roots), B1(H, BitPrefixSumArray(V)), B2(B1), cmp(cmp) {
    sort(S.begin(), S.end(), cmp); if (roots.empty()) {
      for (int v = 0; v < V; v++) if (par[v] == -1) dfs(G, v, -1);
    } else for (int v : roots) dfs(G, v, -1);
    vector<int> C(V); for (int v = 0; v < V; v++)
      C[v] = lower_bound(S.begin(), S.end(), A[v], cmp) - S.begin();
    build(C, pre, mid1, B1); build(C, post, mid2, B2);
  }
  template <class Forest>
  WaveletMatrixTree(const Forest &G, const vector<T> &A, int rt,
                    Cmp cmp = Cmp())
      : WaveletMatrixTree(G, A, vector<int>{rt}, cmp) {}
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
  int query(int h, Ranges &ranges) {
    int val = 0; for (auto &&r : ranges) {
      get<3>(r) = B1[h].query(get<0>(r)); get<4>(r) = B2[h].query(get<1>(r));
      val += get<2>(r) * (get<3>(r) - get<4>(r));
    }
    return val;
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
  template <class F> int cnt(int v, int w, const T &x, F f) {
    Ranges ranges = getRanges(v, w);
    int ret = 0, cur = 0; for (int h = H - 1; h >= 0; h--) {
      int ph = 1 << h, val = query(h, ranges);
      if (cur + ph - 1 >= V || f(x, S[cur + ph - 1])) left(ranges);
      else { cur += ph; ret += val; right(h, ranges); }
    }
    return ret;
  }
  int rank(int v, int w, const T &x) { return cnt(v, w, x, cle); }
  int count(int v, int w, const T &lo, const T &hi) {
    return cnt(v, w, hi, clt) - cnt(v, w, lo, cle);
  }
  T select(int v, int w, int k) {
    Ranges ranges = getRanges(v, w);
    int cur = 0; for (int h = H - 1; h >= 0; h--) {
      int val = query(h, ranges); if (k < val) left(ranges);
      else { cur += 1 << h; k -= val; right(h, ranges); }
    }
    return S[cur];
  }
#undef clt
#undef cle
};
