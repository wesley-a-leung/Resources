#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and range queries in K dimension, even if K is not known at compile time
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   D: a vector of length K with the size of each dimension
//   st: an iterator pointing to the first element in the flattened array
//   en: an iterator pointing to after the last element in the flattened array
// Functions:
//   values(): returns a vector of the fenwick tree decomposed into a
//     flattened array
//   update(ind, v): updates the indices specified by the vector ind holding
//     the index for each dimension, with the value v
//   query(ind): queries the indices specified by the vector ind holding a
//     pair specifying the range for each dimension
// Small constant, like most fenwick trees, slower than FenwickTree.h
// Time Complexity:
//   constructor: O(PI(N_i))
//   update: O(PI(log(N_i)))
//   query: O(2^K * PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
//   PI is the product function, N_i is the size in the ith dimension
// Tested:
//   https://dmoj.ca/problem/inaho2
//   https://dmoj.ca/problem/inaho7
template <class T> struct FenwickTreeKD {
  int K; vector<int> D, suf; vector<T> BIT;
  FenwickTreeKD(const vector<int> &D) : K(D.size()), D(D), suf(K + 1, 1) {
    for (int i = K - 1; i >= 0; i--) suf[i] = suf[i + 1] * D[i];
    BIT.resize(suf[0]);
  }
  int dig(int i, int d) { return i % suf[d] / suf[d + 1]; }
  int nxt(int i) { i++; i += i & -i; return --i; }
  template <class It> FenwickTreeKD(const vector<int> &D, It st, It en)
      : FenwickTreeKD(D) {
    assert(en - st == suf[0]); copy(st, en, BIT.begin());
    for (int d = 0; d < K; d++) for (int i = 0; i < suf[0]; i++)
      if (nxt(dig(i, d)) < D[d])
        BIT[i + (nxt(dig(i, d)) - dig(i, d)) * suf[d + 1]] += BIT[i];
  }
  vector<T> values() {
    vector<T> ret = BIT; for (int d = K - 1; d >= 0; d--)
      for (int i = suf[0] - 1; i >= 0; i--) if (nxt(dig(i, d)) < D[d])
        ret[i + (nxt(dig(i, d)) - dig(i, d)) * suf[d + 1]] -= ret[i];
    return ret;
  }
  void update(const vector<int> &ind, const T &v, int d = 0, int pos = 0) {
    if (d == K) BIT[pos] += v;
    else for (int i = ind[d] + 1; i <= D[d]; i += i & -i)
      update(ind, v, d + 1, pos + (i - 1) * suf[d + 1]);
  }
  T query(const vector<pair<int, int>> &ind, int d = 0, int pos = 0) {
    T ret = T(); if (d == K) ret += BIT[pos];
    else {
      for (int l = ind[d].first; l > 0; l -= l & -l)
        ret -= query(ind, d + 1, pos + (l - 1) * suf[d + 1]);
      for (int r = ind[d].second + 1; r > 0; r -= r & -r)
        ret += query(ind, d + 1, pos + (r - 1) * suf[d + 1]);
    }
    return ret;
  }
};
