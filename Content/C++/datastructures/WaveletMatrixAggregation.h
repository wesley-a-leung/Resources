#pragma once
#include <bits/stdc++.h>
#include "BitPrefixSumArray.h"
using namespace std;

// Wavelet Matrix supporting 2D aggregation queries where the data can change,
//   but not the keys
// Indices are 0-indexed and ranges are inclusive
// Queries bounded by 3 indices require a commutative operation, while queries
//   bounded by 4 indices also requires the data to be invertible
// Template Arguments:
//   T: the type of the element of the array
//   R: struct supporting point updates and range queries on indices
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
//     Sample Struct: supporting point increments updates and range sum queries
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
//   Cmp: the comparator to compare two elements of type T
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   A: a vector of type T containing the initial keys of the array
//   X: a vector of type R::Data containing the initial data of the array
//   cmp: an instance of the Cmp struct
// Functions:
//   update(i, v): updates the ith data with the lazy value v
//   query(l, r, hi): returns the aggregate value of the data associated with
//     all elements with a key less than or equal to hi (using the comparator)
//     in the range [l, r]
//   query(l, r, lo, hi): returns the aggregate value of the data associated
//     with all elements with a key of not less than lo and not greater than hi
//     (using the comparator) in the range [l, r]
//   bsearch(l, r, f): over all keys in the array, finds the first key k such
//     that query(l, r, k) returns true
// In practice, has a small constant, faster than using a
//   2D Sparse Fenwick Tree or Segment Tree
// Time Complexity:
//   constructor: O((N + C) log N) where C is the time complexity of
//     R's constructor
//   update: O(U log N) where U is the time complexity of R's update function
//   query, bsearch: O(Q log N) where Q is the time complexity of
//     R's query function
// Memory Complexity: O(N + (N log N) / 64 + M log N) where M is the memory
//   complexity of R
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/dmopc19c7p5
template <class T, class R, class Cmp = less<T>>
struct WaveletMatrixAggregation {
#define clt [&] (const T &a, const T &b) { return cmp(a, b); }
#define cle [&] (const T &a, const T &b) { return !cmp(b, a); }
  using Data = typename R::Data; using Lazy = typename R::Lazy;
  int N, H; Cmp cmp; vector<int> mid; vector<BitPrefixSumArray> B;
  vector<R> D; vector<T> S;
  WaveletMatrixAggregation(vector<T> A, const vector<Data> &X, Cmp cmp = Cmp())
      : N(A.size()), H(N == 0 ? 0 : __lg(N) + 1), cmp(cmp), mid(H),
        B(H, BitPrefixSumArray(N)), S(move(A)) {
    vector<T> temp = S; sort(S.begin(), S.end(), cmp);
    vector<int> C(N), ind(N); for (int i = 0; i < N; i++)
      C[i] = lower_bound(S.begin(), S.end(), temp[i], cmp) - S.begin();
    iota(ind.begin(), ind.end(), 0); D.reserve(H); vector<Data> Y = X;
    for (int h = H - 1; h >= 0; h--) {
      int ph = 1 << h;
      for (int i = 0; i < N; i++) B[h].set(i, C[ind[i]] <= ph - 1); 
      mid[h] = stable_partition(ind.begin(), ind.end(), [&] (int i) {
                                  return C[i] <= ph - 1;
                                }) - ind.begin();
      B[h].build(); for (int i = 0; i < N; i++) Y[i] = X[ind[i]];
      D.emplace_back(Y); for (int i = mid[h]; i < N; i++) C[ind[i]] -= ph;
    }
    reverse(D.begin(), D.end());
  }
  void update(int i, const Lazy &v) {
    for (int h = H - 1; h >= 0; h--) {
      if (B[h].get(i)) i = B[h].query(i - 1);
      else i += mid[h] - B[h].query(i - 1);
      D[h].update(i, v);
    }
  }
  template <class F>
  Data qryPre(int h, int cur, int l, int r, const T &v, F f) {
    Data ret = R::qdef(); for (; h >= 0; h--) {
      int ph = 1 << h, ql = B[h].query(l - 1), qr = B[h].query(r);
      if (cur + ph - 1 >= N || f(v, S[cur + ph - 1])) { l = ql; r = qr - 1; }
      else {
        if (ql < qr) ret = R::merge(ret, D[h].query(ql, qr - 1));
        cur += ph; l += mid[h] - ql; r += mid[h] - qr;
      }
    }
    return ret;
  }
  template <class F>
  Data qrySuf(int h, int cur, int l, int r, const T &v, F f) {
    Data ret = R::qdef(); for (; h >= 0; h--) {
      int ph = 1 << h, ql = B[h].query(l - 1), qr = B[h].query(r);
      if (cur + ph - 1 >= N || f(v, S[cur + ph - 1])) {
        if (l - ql <= r - qr)
          ret = R::merge(ret, D[h].query(l + mid[h] - ql, r + mid[h] - qr));
        if (h == 0 && ql < qr) ret = R::merge(ret, D[h].query(ql, qr - 1));
        l = ql; r = qr - 1;
      } else {
        if (h == 0 && l - ql <= r - qr)
          ret = R::merge(ret, D[h].query(l + mid[h] - ql, r + mid[h] - qr));
        cur += ph; l += mid[h] - ql; r += mid[h] - qr;
      }
    }
    return ret;
  }
  Data query(int l, int r, const T &hi) {
    return qryPre(H - 1, 0, l, r, hi, clt);
  }
  Data query(int l, int r, const T &lo, const T &hi) {
    for (int cur = 0, h = H - 1; h >= 0; h--) {
      int ph = 1 << h, ql = B[h].query(l - 1), qr = B[h].query(r);
      bool loLeft = cur + ph - 1 >= N || !cmp(S[cur + ph - 1], lo);
      bool hiLeft = cur + ph - 1 >= N || cmp(hi, S[cur + ph - 1]);
      if (loLeft != hiLeft) {
        Data ret = R::merge(qrySuf(h - 1, cur, ql, qr - 1, lo, cle),
                            qryPre(h - 1, cur + ph, l + mid[h] - ql,
                                   r + mid[h] - qr, hi, clt));
        return h == 0 && ql < qr ? R::merge(ret, D[h].query(ql, qr - 1)) : ret;
      } else if (loLeft) { l = ql; r = qr - 1; }
      else { cur += ph; l += mid[h] - ql; r += mid[h] - qr; }
    }
    return R::qdef();
  }
  template <class F> pair<bool, T *> bsearch(int l, int r, F f) {
    int cur = 0; Data agg = R::qdef(); for (int h = H - 1; h >= 0; h--) {
      int ql = B[h].query(l - 1), qr = B[h].query(r);
      Data val = ql < qr ? D[h].query(ql, qr - 1) : R::qdef();
      if (f(R::merge(agg, val))) { l = ql; r = qr - 1; }
      else {
        cur += 1 << h; agg = R::merge(agg, val);
        l += mid[h] - ql; r += mid[h] - qr;
      }
    }
    return make_pair(cur < N, cur < N ? &S[cur] : nullptr);
  }
#undef clt
#undef cle
};
