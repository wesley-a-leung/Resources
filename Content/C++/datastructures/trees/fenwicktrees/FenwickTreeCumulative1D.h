#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and prefix range queries over a cumulative function or functor,
//   such as max and min in 1 dimension
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of each element
//   Op: a struct with the cumulative operation
//     Required Functions:
//       operator (l, r): combines the values l and r
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
//   qdef: the identity element of the operation
//   op: an instance of the Op struct
// Functions:
//   update(i, v): modifies the value A[i] to op(A[i], v)
//   query(r): queries the cumulative value of the range [0, r]
//   bsearch(v, cmp): returns the first index where cmp(op(A[0..i]), v)
//     returns false, or N if no such index exists, assumes A is sorted by cmp
// In practice, this version performs as well as the multidimensional version
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor: O(N)
//   update, query: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://atcoder.jp/contests/dp/tasks/dp_q
//   https://dmoj.ca/problem/ds1
//   https://dmoj.ca/problem/cco10p3
template <class T, class C> struct FenwickTreeCumulative1D {
  int N; vector<T> BIT; C op;
  FenwickTreeCumulative1D(int N, T qdef, C op = C())
      : N(N), BIT(N + 1, qdef), op(op) {}
  template <class F> FenwickTreeCumulative1D(int N, F f, T qdef, C op = C())
      : FenwickTreeCumulative1D(N, qdef, op) {
    for (int i = 1; i <= N; i++) {
      BIT[i] = op(BIT[i], f());
      int j = i + (i & -i); if (j <= N) BIT[j] = op(BIT[j], BIT[i]);
    }
  }
  template <class It>
  FenwickTreeCumulative1D(It st, It en, T qdef, C op = C())
      : FenwickTreeCumulative1D(en - st, [&] { return *st++; }, qdef, op) {}
  void update(int i, T v) {
    for (i++; i <= N; i += i & -i) BIT[i] = op(BIT[i], v);
  }
  T query(int r) {
    T ret = BIT[0]; for (r++; r > 0; r -= r & -r) ret = op(ret, BIT[r]);
    return ret;
  }
  template <class F> int bsearch(T v, F cmp) {
    T agg = BIT[0]; int ind = 0; for (int j = __lg(N + 1); j >= 0; j--) {
      int i = ind + (1 << j);
      if (i <= N && cmp(op(agg, BIT[i]), v)) agg = op(agg, BIT[ind = i]);
    }
    return ind;
  }
};
