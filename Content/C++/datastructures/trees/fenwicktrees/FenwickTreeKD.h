#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting point updates
//   and range queries in K dimension, even if K is not known at copile time
// Indices are 0-indexed and ranges are inclusive
// Template Arguments:
//   T: the type of each element
// Constructor Arguments:
//   D: a vector of length K with the size of each dimension
// Functions:
//   update(ind, v): updates the indices spcified by the vector ind holding the
//     index for each dimension, with the value v
//   query(ind): queries the indices spcified by the vector ind holding a pair
//     specifying the range for each dimension
// Small constant, like most fenwick trees
// Time Complexity:
//   constructor: O(PI(N_i))
//   update: O(PI(log(N_i)))
//   query: O(2^K * PI(log(N_i)))
// Memory Complexity: O(PI(N_i))
//   PI is the product function, N_i is the size in the ith dimension
// Tested:
//   https://dmoj.ca/problem/inaho2
template <class T> struct FenwickTreeKD {
  vector<int> D, suf; vector<T> BIT;
  FenwickTreeKD(const vector<int> &D) : D(D), suf(D.size()) {
    int prod = 1; for (int i = int(D.size()) - 1; i >= 0; i--) {
      suf[i] = prod; prod *= D[i];
    }
    BIT.resize(prod);
  }
  void update(const vector<int> &ind, const T &v, int d = 0, int pos = 0) {
    if (d == int(D.size())) BIT[pos] += v;
    else for (int i = ind[d] + 1; i <= D[d]; i += i & -i)
      update(ind, v, d + 1, pos + (i - 1) * suf[d]);
  }
  T query(const vector<pair<int, int>> &ind, int d = 0, int pos = 0) {
    T ret = T(); if (d == int(D.size())) ret += BIT[pos];
    else {
      for (int l = ind[d].first; l > 0; l -= l & -l)
        ret -= query(ind, d + 1, pos + (l - 1) * suf[d]);
      for (int r = ind[d].second + 1; r > 0; r -= r & -r)
        ret += query(ind, d + 1, pos + (r - 1) * suf[d]);
    }
    return ret;
  }
};
