#pragma once
#include <bits/stdc++.h>
using namespace std;

// Prefix Sum Array supporting range sum queries where all values are 0 or 1,
//   and all values are set before all queries
// Indices are 0-indexed and ranges are inclusive
// Constructor Arguments:
//   N: the size of the array
// Functions:
//   set(i, v): sets the index i with the boolean value v
//   get(i): gets the value of index i
//   build(): builds the prefix sum array, must be called before query can
//     be called after a set operation
//   query(r): queries the sum of the range [0, r]
//   query(l, r): queries the sum of the range [l, r]
// In practice, has a moderate constant, performs faster than a regular prefix
//   sum array and uses less memory
// Time Complexity:
//   constructor, set, get, query: O(1)
//   build: O(N / 64)
// Memory Complexity: O(N / 64)
// Tested:
//   Fuzz and Stress Tested
struct BitPrefixSum {
  int M; vector<uint64_t> mask; vector<int> pre;
  BitPrefixSum(int N) : M((N >> 6) + 1), mask(M, 0), pre(M + 1, 0) {}
  void set(int i, bool v) {
    int j = i >> 6, k = i & 63;
    mask[j] = (mask[j] & ~(uint64_t(1) << k)) | (uint64_t(v) << k);
  }
  bool get(int i) { return (mask[i >> 6] >> (i & 63)) & 1; }
  void build() {
    for (int j = 0; j < M; j++)
      pre[j + 1] = pre[j] + __builtin_popcountll(mask[j]);
  }
  int query(int r) {
    r++; int j = r >> 6, k = r & 63;
    return pre[j] + __builtin_popcountll(mask[j] & ((uint64_t(1) << k) - 1));
  }
  int query(int l, int r) { return query(r) - query(l - 1); }
};
