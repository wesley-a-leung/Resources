#pragma once
#include <bits/stdc++.h>
#include "../../datastructures/BitPrefixSumArray.h"
#include "HeavyLightDecomposition.h"
using namespace std;

// Wavelet Matrix supporting 2D aggregation path queries using heavy light
//   decomposition on a forest where the data can change, but not the keys
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
//       constructor(A): takes a vector A of type Data with the initial
//         value of each index
//       update(i, val): updates the index i with the value val
//       query(l, r): queries the range [l, r]
//     Sample Struct: supporting point increments updates and
//         prefix sum queries
//       struct R {
//         using Data = int;
//         using Lazy = int;
//         static Data qdef() { return 0; }
//         static Data merge(const Data &l, const Data &r) { return l + r; }
//         FenwickTree1D<Data> FT;
//         R(const vector<Data> &A) : FT(A) {}
//         void update(int i, const Lazy &val) { FT.update(i, val); }
//         Data query(int l, int r) { return FT.query(l, r); }
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
//   query, bsearch: O(Q (log V)^2) where Q is the time complexity of
//     R's query function
// Memory Complexity: O(V + (V log V) / 64 + M log V) where M is the memory
//   complexity of R
// Tested:
//   Fuzz and Stress Tested
template <class T, class R, const bool VALUES_ON_EDGES, class Cmp = less<T>>
struct WaveletMatrixHLD : public HLD {
#define clt [&] (const T &a, const T &b) { return cmp(a, b); }
#define cle [&] (const T &a, const T &b) { return !cmp(b, a); }
  using Data = typename R::Data; using Lazy = typename R::Lazy;
  int H; vector<int> mid; vector<T> S; vector<BitPrefixSumArray> B;
  vector<R> D; Cmp cmp;
  template <class Forest>
  WaveletMatrixHLD(const Forest &G, const vector<T> &A, const vector<Data> &X,
                   const vector<int> &roots = vector<int>(), Cmp cmp = Cmp())
      : HLD(G, roots), H(V == 0 ? 0 : __lg(V) + 1), mid(H), S(A),
        B(H, BitPrefixSumArray(V)), cmp(cmp) {
    sort(S.begin(), S.end(), cmp); vector<int> temp(V), C(V), P(V);
    vector<Data> Y = X, Z = X; D.reserve(H); for (int v = 0; v < V; v++)
      temp[v] = lower_bound(S.begin(), S.end(), A[v], cmp) - S.begin();
    for (int v = 0; v < V; v++) { C[pre[v]] = temp[v]; Y[pre[v]] = X[v]; }
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
  WaveletMatrixHLD(const Forest &G, const vector<T> &A,
                   const vector<Data> &X, int rt, Cmp cmp = Cmp())
      : WaveletMatrixHLD(G, A, X, vector<int>{rt}, cmp) {}
  using Ranges = vector<tuple<int, int, int, int>>;
  Ranges getRanges(int v, int w) {
    Ranges ranges; while (head[v] != head[w]) {
      if (dep[head[v]] < dep[head[w]]) {
        ranges.emplace_back(pre[head[w]], pre[w], 0, 0); w = par[head[w]];
      } else {
        ranges.emplace_back(pre[head[v]], pre[v], 0, 0); v = par[head[v]];
      }
    }
    if (v != w) {
      if (dep[v] < dep[w])
        ranges.emplace_back(pre[v] + VALUES_ON_EDGES, pre[w], 0, 0);
      else ranges.emplace_back(pre[w] + VALUES_ON_EDGES, pre[v], 0, 0);
    } else if (!VALUES_ON_EDGES) {
      int i = pre[dep[v] < dep[w] ? v : w]; ranges.emplace_back(i, i, 0, 0);
    }
    return ranges;
  }
  void update(int v, const Lazy &val) {
    for (int i = pre[v], h = H - 1; h >= 0; h--) {
      if (B[h].get(i)) i = B[h].query(i - 1);
      else i += mid[h] - B[h].query(i - 1);
      D[h].update(i, val);
    }
  }
  void qryRanges(int h, Ranges &ranges) {
    for (auto &&r : ranges) {
      get<2>(r) = B[h].query(get<0>(r) - 1); get<3>(r) = B[h].query(get<1>(r));
    }
  }
  Data qryAggLeft(int h, const Ranges &ranges) {
    Data ret = R::qdef(); for (auto &&r : ranges) {
      int a = get<2>(r), b = get<3>(r) - 1;
      if (a <= b) ret = R::merge(ret, D[h].query(a, b));
    }
    return ret;
  }
  Data qryAggRight(int h, const Ranges &ranges) {
    Data ret = R::qdef(); for (auto &&r : ranges) {
      int a = get<0>(r) + mid[h] - get<2>(r);
      int b = get<1>(r) + mid[h] - get<3>(r);
      if (a <= b) ret = R::merge(ret, D[h].query(a, b));
    }
    return ret;
  }
  void left(Ranges &ranges) {
    for (auto &&r : ranges) {
      get<0>(r) = get<2>(r); get<1>(r) = get<3>(r) - 1;
    }
  }
  void right(int h, Ranges &ranges) {
    for (auto &&r : ranges) {
      get<0>(r) += mid[h] - get<2>(r); get<1>(r) += mid[h] - get<3>(r);
    }
  }
  template <class F>
  Data qryPre(int h, int cur, Ranges &ranges, const T &x, F f) {
    Data ret = R::qdef(); for (; h >= 0; h--) {
      int ph = 1 << h; qryRanges(h, ranges);
      if (cur + ph - 1 >= V || f(x, S[cur + ph - 1])) left(ranges);
      else {
        ret = R::merge(ret, qryAggLeft(h, ranges));
        cur += ph; right(h, ranges);
      }
    }
    return ret;
  }
  template <class F>
  Data qrySuf(int h, int cur, Ranges &ranges, const T &v, F f) {
    Data ret = R::qdef(); for (; h >= 0; h--) {
      int ph = 1 << h; qryRanges(h, ranges);
      if (cur + ph - 1 >= V || f(v, S[cur + ph - 1])) {
        ret = R::merge(ret, qryAggRight(h, ranges));
        if (h == 0) ret = R::merge(ret, qryAggLeft(h, ranges));
        left(ranges);
      } else {
        if (h == 0) ret = R::merge(ret, qryAggRight(h, ranges));
        cur += ph; right(h, ranges);
      }
    }
    return ret;
  }
  Data query(int v, int w, const T &hi) {
    Ranges ranges = getRanges(v, w); return qryPre(H - 1, 0, ranges, hi, clt);
  }
  Data query(int v, int w, const T &lo, const T &hi) {
    Ranges ranges = getRanges(v, w);
    for (int cur = 0, h = H - 1; h >= 0; h--) {
      int ph = 1 << h; qryRanges(h, ranges);
      bool loLeft = cur + ph - 1 >= V || !cmp(S[cur + ph - 1], lo);
      bool hiLeft = cur + ph - 1 >= V || cmp(hi, S[cur + ph - 1]);
      if (loLeft != hiLeft) {
        Ranges leftRanges = ranges, rightRanges = ranges;
        left(leftRanges); right(h, rightRanges);
        Data ret = R::merge(qrySuf(h - 1, cur, leftRanges, lo, cle),
                            qryPre(h - 1, cur + ph, rightRanges, hi, clt));
        return h == 0 ? R::merge(ret, qryAggLeft(h, ranges)) : ret;
      } else if (loLeft) left(ranges);
      else { cur += ph; right(h, ranges); }
    }
    return R::qdef();
  }
  template <class F> pair<bool, T *> bsearch(int v, int w, F f) {
    int cur = 0; Data agg = R::qdef(); Ranges ranges = getRanges(v, w);
    for (int h = H - 1; h >= 0; h--) {
      qryRanges(h, ranges); Data val = qryAggLeft(h, ranges);
      if (f(R::merge(agg, val))) left(ranges);
      else { cur += 1 << h; agg = R::merge(agg, val); right(h, ranges); }
    }
    return make_pair(cur < V, cur < V ? &S[cur] : nullptr);
  }
#undef clt
#undef cle
};
