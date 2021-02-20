#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and range queries in 1 dimension
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Functions:
//   values(): returns a vector of the fenwick tree decomposed into an array
//   update(i, v): add v to the value at index i
//   query(r): queries the range [0, r]
//   query(l, r): queries the range [l, r]
//   bsearch(v, cmp): returns the first index where cmp(sum(A[0..i]), v)
//     returns false, or N if no such index exists
//   lower_bound(v): returns the first index where sum(A[0..i]) >= v, assumes
//     cmp(sum(A[0..i + 1]), sum(A[0..i])) returns false, returns N if no such
//     index exists
//   upper_bound(v): returns the first index where sum(A[0..i]) > v, assumes
//     cmp(sum(A[0..i + 1]), sum(A[0..i])) returns false, returns N if no such
//     index exists
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor, values: O(N)
//   update, query, bsearch, lower_bound, upper_bound: O(log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://dmoj.ca/problem/ds1
//   https://judge.yosupo.jp/problem/point_add_range_sum
//   https://codeforces.com/contest/1279/problem/B
//   https://dmoj.ca/problem/ccc05s5
//   https://dmoj.ca/problem/cco10p3
//   https://dmoj.ca/problem/apio19p3
template <class T> struct FenwickTree1D {
  int N; vector<T> BIT; FenwickTree1D(int N) : N(N), BIT(N + 1, T()) {}
  template <class F> FenwickTree1D(int N, F f) : FenwickTree1D(N) {
    for (int i = 1; i <= N; i++) {
      BIT[i] += f(); int j = i + (i & -i); if (j <= N) BIT[j] += BIT[i];
    }
  }
  template <class It> FenwickTree1D(It st, It en)
      : FenwickTree1D(en - st, [&] { return *st++; }) {}
  vector<T> values() {
    vector<T> ret(BIT.begin() + 1, BIT.end()); for (int i = N; i >= 1; i--) {
      int j = i + (i & -i); if (j <= N) ret[j - 1] -= ret[i - 1];
    }
    return ret;
  }
  void update(int i, T v) { for (i++; i <= N; i += i & -i) BIT[i] += v; }
  T query(int r) {
    T ret = T(); for (r++; r > 0; r -= r & -r) ret += BIT[r];
    return ret;
  }
  T query(int l, int r) { return query(r) - query(l - 1); }
  template <class F> int bsearch(T v, F cmp) {
    T sum = T(); int ind = 0; for (int j = __lg(N + 1); j >= 0; j--) {
      int i = ind + (1 << j);
      if (i <= N && cmp(sum + BIT[i], v)) sum += BIT[ind = i];
    }
    return ind;
  }
  int lower_bound(T v) { return bsearch(v, less<T>()); }
  int upper_bound(T v) { return bsearch(v, less_equal<T>()); }
};
