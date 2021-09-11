#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates
//   and point queries, over a cumulative functor, such as sum, max, and min,
//   in 1 dimension
// Indices are 0-indexed and ranges are inclusive
// Order of arguments for update differs slightly from FenwickTreeRangePoint
// Template Arguments:
//   T: the type of each element
//   Op: a struct with the cumulative operation (plus<T> by default)
//     Required Functions:
//       operator (l, r): combines the values l and r, must be associative and
//         commutatitve
// Constructor Arguments:
//   N: the size of the array
//   A: a vector of type T, memory is saved if this is moved and has
//     a capacity of N + 1
//   qdef: the identity element of the operation
//   op: an instance of the Op struct
//   inv(l, r): a functor or function that performs the inverse operation of
//     op (minus<T>() by default)
// Functions:
//   values(inv): returns a vector of the fenwick tree decomposed into an
//     array, where inv is the inverse of op (minus<T>() by default)
//   update(l, v): sets A[i] to op(A[i], v) for the range [l..N - 1]
//   update(l, r, v, inv): add A[i] to op(A[i], v) for the range [l..r]
//   get(i): queries the value at index i, where inv is the inverse of
//     op (minus<T>() by default)
//   bsearch(v, cmp): returns the first index where cmp(A[i], v)
//     returns false, or N if no such index exists
// In practice, this version performs as well as the multidimensional version
// In practice, has a small constant, faster than segment trees
// Time Complexity:
//   constructor, values: O(N)
//   update, get: O(log N)
// Memory Complexity: O(N)
// Tested:
//   Fuzz and Stress Tested
//   https://mcpt.ca/problem/asquirrelproblem
//   https://codeforces.com/contest/1404/problem/C
template <class T, class Op = plus<T>> struct FenwickTreeRangePoint1D {
  int N; vector<T> BIT; Op op;
  FenwickTreeRangePoint1D(int N, T qdef = T(), Op op = Op())
      : N(N), BIT(N + 1, qdef), op(op) {}
  template <class Inv = minus<T>>
  FenwickTreeRangePoint1D(vector<T> A, T qdef = T(),
                          Op op = Op(), Inv inv = Inv())
      : N(A.size()), BIT(move(A)), op(op) {
    adjacent_difference(BIT.begin(), BIT.end(), BIT.begin(), inv);
    BIT.reserve(N + 1); BIT.insert(BIT.begin(), qdef);
    for (int i = 1; i <= N; i++) {
      int j = i + (i & -i); if (j <= N) BIT[j] = op(BIT[j], BIT[i]);
    }
  }
  template <class Inv = minus<T>> vector<T> values(Inv inv = Inv()) {
    vector<T> ret(BIT.begin() + 1, BIT.end()); for (int i = N; i >= 1; i--) {
      int j = i + (i & -i);
      if (j <= N) ret[j - 1] = inv(ret[j - 1], ret[i - 1]);
    }
    for (int i = 1; i < N; i++) ret[i] = op(ret[i - 1], ret[i]);
    return ret;
  }
  void update(int l, T v) {
    for (l++; l <= N; l += l & -l) BIT[l] = op(BIT[l], v);
  }
  template <class Inv = minus<T>>
  void update(int l, int r, T v, Inv inv = Inv()) {
    update(l, v); update(r + 1, inv(BIT[0], v));
  }
  T get(int i) {
    T ret = BIT[0]; for (i++; i > 0; i -= i & -i) ret = op(ret, BIT[i]);
    return ret;
  }
  template <class F> int bsearch(T v, F cmp) {
    T val = BIT[0]; int ind = 0; for (int j = __lg(N + 1); j >= 0; j--) {
      int i = ind + (1 << j);
      if (i <= N && cmp(op(val, BIT[i]), v)) val = op(val, BIT[ind = i]);
    }
    return ind;
  }
};
