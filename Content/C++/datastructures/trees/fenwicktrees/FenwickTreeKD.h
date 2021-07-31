#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and range queries over a cumulative function or functor,
//   such as max and min, in K dimensions, even if K is not known at
//   compile time
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutatitve
// Constructor Arguments:
//   D: a vector of length K with the size of each dimension
//   st: an iterator pointing to the first element in the flattened array
//   en: an iterator pointing to after the last element in the flattened array
// Functions:
//   values(inv): returns a vector of the fenwick tree decomposed into a
//     flattened array, where inv is the inverse of op (minus<T>() by default)
//   update(ind, v): updates the indices specified by the vector ind holding
//     the index for each dimension, with the value a being modified to
//     op(a, v)
//   query(ind): queries the cumulative value of the prefix bounded by the
//     indices specified by the vector ind holding the index for each
//     dimension, where inv is the inverse of Op (minus<T> by default)
//   query(ind, inv): queries the cumulative value of the indices specified by
//     the vector ind holding a pair specifying the range for each dimension,
//     where inv is the inverse of Op (minus<T> by default)
// In practice, has a small constant, slower than FenwickTree.h
// Time Complexity:
//   constructor: O(PI(N_i))
//   update, query(ind): O(PI(log(N_i)))
//   query(ind, inv): O(2^K * PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
//   PI is the product function, N_i is the size in the ith dimension
// Tested:
//   https://dmoj.ca/problem/inaho2
//   https://dmoj.ca/problem/inaho7
template <class T, class Op = plus<T>> struct FenwickTreeKD {
  int K; vector<int> D, suf; vector<T> BIT; T qdef; Op op;
  FenwickTreeKD(const vector<int> &D, T qdef = T(), Op op = Op())
      : K(D.size()), D(D), suf(K + 1, 1), qdef(qdef), op(op) {
    for (int i = K - 1; i >= 0; i--) suf[i] = suf[i + 1] * D[i];
    BIT.resize(suf[0]);
  }
  int dig(int i, int d) { return i % suf[d] / suf[d + 1]; }
  int nxt(int i) { i++; i += i & -i; return --i; }
  template <class It> FenwickTreeKD(const vector<int> &D, It st, It en)
      : FenwickTreeKD(D) {
    assert(en - st == suf[0]); copy(st, en, BIT.begin());
    for (int d = 0; d < K; d++) for (int i = 0; i < suf[0]; i++)
      if (nxt(dig(i, d)) < D[d]) {
        int j = i + (nxt(dig(i, d)) - dig(i, d)) * suf[d + 1];
        BIT[j] = op(BIT[j], BIT[i]);
      }
  }
  template <class Inv = minus<T>> vector<T> values(Inv inv = Inv()) {
    vector<T> ret = BIT; for (int d = K - 1; d >= 0; d--)
      for (int i = suf[0] - 1; i >= 0; i--) if (nxt(dig(i, d)) < D[d]) {
        int j = i + (nxt(dig(i, d)) - dig(i, d)) * suf[d + 1];
        ret[j] = inv(ret[j], ret[i]);
      }
    return ret;
  }
  void update(const vector<int> &ind, const T &v, int d = 0, int pos = 0) {
    if (d == K) BIT[pos] = op(BIT[pos], v);
    else for (int i = ind[d] + 1; i <= D[d]; i += i & -i)
      update(ind, v, d + 1, pos + (i - 1) * suf[d + 1]);
  }
  T query(const vector<int> &ind, int d = 0, int pos = 0) {
    T ret = qdef; if (d == K) ret = op(ret, BIT[pos]);
    else for (int r = ind[d] + 1; r > 0; r -= r & r)
      ret = op(ret, query(ind, d + 1, pos + (r - 1) * suf[d + 1]));
    return ret;
  }
  template <class Inv = minus<T>>
  T query(const vector<pair<int, int>> &ind, Inv inv = Inv(),
          int d = 0, int pos = 0) {
    T ret = qdef; if (d == K) ret = op(ret, BIT[pos]);
    else {
      for (int l = ind[d].first; l > 0; l -= l & -l)
        ret = inv(ret, query(ind, inv, d + 1, pos + (l - 1) * suf[d + 1]));
      for (int r = ind[d].second + 1; r > 0; r -= r & -r)
        ret = op(ret, query(ind, inv, d + 1, pos + (r - 1) * suf[d + 1]));
    }
    return ret;
  }
};
