#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and range queries over a cumulative function or functor,
//   such as sum, max, and min, in 1 dimension
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of each element
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutatitve
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   qdef: the identity element of the operation
//   op: an instance of the Op struct
// Functions:
//   values(inv): returns a vector of the fenwick tree decomposed into an
//     array, where inv is the inverse of op (minus<T>() by default)
//   update(i, v): modifies the value A[i] to op(A[i], v)
//   query(r): queries the cumulative value of the range [0, r]
//   query(l, r, inv): queries the cumulative value of the range [l, r], where
//     inv is the inverse of op (minus<T>() by default)
//   bsearch(v, cmp): returns the first index where cmp(op(A[0..i]), v)
//     returns false, or N if no such index exists
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor, values: O(N)
//   update, query, bsearch: O(log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/ds1
//   https://judge.yosupo.jp/problem/point_add_range_sum
//   https://atcoder.jp/contests/dp/tasks/dp_q
template <class T, class Op = plus<T>> struct FenwickTree1D {
  int N; vector<T> BIT; Op op;
  FenwickTree1D(int N, T qdef = T(), Op op = Op())
      : N(N), BIT(N + 1, qdef), op(op) {}
  template <class F> FenwickTree1D(int N, F f, T qdef = T(), Op op = Op())
      : FenwickTree1D(N, qdef, op) {
    for (int i = 1; i <= N; i++) {
      BIT[i] = op(BIT[i], f());
      int j = i + (i & -i); if (j <= N) BIT[j] = op(BIT[j], BIT[i]);
    }
  }
  template <class It> FenwickTree1D(It st, It en, T qdef = T(), Op op = Op())
      : FenwickTree1D(en - st, [&] { return *st++; }, qdef, op) {}
  template <class Inv = minus<T>> vector<T> values(Inv inv = Inv()) {
    vector<T> ret(BIT.begin() + 1, BIT.end()); for (int i = N; i >= 1; i--) {
      int j = i + (i & -i);
      if (j <= N) ret[j - 1] = inv(ret[j - 1], ret[i - 1]);
    }
    return ret;
  }
  void update(int i, T v) {
    for (i++; i <= N; i += i & -i) BIT[i] = op(BIT[i], v);
  }
  T query(int r) {
    T ret = BIT[0]; for (r++; r > 0; r -= r & -r) ret = op(ret, BIT[r]);
    return ret;
  }
  template <class Inv = minus<T>> T query(int l, int r, Inv inv = Inv()) {
    return inv(query(r), query(l - 1));
  }
  template <class F> int bsearch(T v, F cmp) {
    T agg = BIT[0]; int ind = 0; for (int j = __lg(N + 1); j >= 0; j--) {
      int i = ind + (1 << j);
      if (i <= N && cmp(op(agg, BIT[i]), v)) agg = op(agg, BIT[ind = i]);
    }
    return ind;
  }
};
