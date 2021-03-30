#pragma once
#include <bits/stdc++.h>
#include "FenwickTree1D.h"
using namespace std;

// Fenwick Tree supporting range sum queries where all values are 0 or 1,
//   and values can be updated after they are set
// Indices are 0-indexed and ranges are inclusive
// Constructor Arguments:
//   N: the size of the array
// Functions:
//   set(i, v): sets the initial value of index i with the boolean value v
//   get(i): gets the current value of index i
//   build(): builds the fenwick tree, set should not be called afterwards
//   update(i, v): updates the value of index i with the boolean value v,
//     should not be called before build unless there were no calls to set
//   query(r): queries the sum of the range [0, r]
//   query(l, r): queries the sum of the range [l, r]
// In practice, has a moderate constant, performs faster than a regular fenwick
//   tree and uses less memory
// Time Complexity:
//   constructor, set, get: O(1)
//   update, query: O(log(N / 64))
//   build: O(N / 64)
// Memory Complexity: O(N / 64)
// Tested:
//   Fuzz and Stress Tested
struct BitFenwickTree {
  int M; vector<uint64_t> mask; vector<int> BIT;
  BitFenwickTree(int N) : M((N >> 6) + 1), mask(M, 0), BIT(M + 1, 0) {}
  void set(int i, bool v) {
    int j = i >> 6, k = i & 63;
    mask[j] = (mask[j] & ~(uint64_t(1) << k)) | (uint64_t(v) << k);
  }
  bool get(int i) { return (mask[i >> 6] >> (i & 63)) & 1; }
  void build() {
    for (int i = 1; i <= M; i++) {
      BIT[i] += __builtin_popcountll(mask[i - 1]); int j = i + (i & -i);
      if (j <= M) BIT[j] += BIT[i];
    }
  }
  void update(int i, bool v) {
    int j = i >> 6, k = i & 63; if (((mask[j] >> k) & 1) != v) {
      mask[j] ^= uint64_t(1) << k; int delta = v ? 1 : -1;
      for (j++; j <= M; j += j & -j) BIT[j] += delta;
    }
  }
  int query(int r) {
    r++; int j = r >> 6, k = r & 63, ret = 0;
    for (int i = j; i > 0; i -= i & -i) ret += BIT[i];
    return ret + __builtin_popcountll(mask[j] & ((uint64_t(1) << k) - 1));
  }
  int query(int l, int r) { return query(r) - query(l - 1); }
};
