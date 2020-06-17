#pragma once
#include <bits/stdc++.h>
using namespace std;

// Fenwick Tree or Binary Indexed Tree supporting range updates
//   and point queries in 1 dimension
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// In practice, this version performs as well as the multidimensional version
// Very small constant, like most fenwick trees
// Time Complexity:
//   constructor, values: O(N)
//   update, get: O(log N)
// Memory Complexity: O(N)
// Tested:
//   https://mcpt.ca/problem/asquirrelproblem
//   https://codeforces.com/contest/1254/problem/D
template <class T> struct FenwickTreeRangePoint1D {
  int N; vector<T> BIT;
  FenwickTreeRangePoint1D(int N) : N(N), BIT(N + 1, T()) {}
  template <class It> FenwickTreeRangePoint1D(It st, It en)
      : FenwickTreeRangePoint1D(en - st) {
    adjacent_difference(st, en, BIT.begin() + 1);
    for (int i = 1; i <= N; i++) {
      int j = i + (i & -i); if (j <= N) BIT[j] += BIT[i];
    }
  }
  vector<T> values() {
    vector<T> ret(BIT.begin() + 1, BIT.end()); for (int i = N; i >= 1; i--) {
      int j = i + (i & -i); if (j <= N) ret[j - 1] -= ret[i - 1];
    }
    partial_sum(ret.begin(), ret.end(), ret.begin()); return ret;
  }
  void update(int l, T v) { for (l++; l <= N; l += l & -l) BIT[l] += v; }
  void update(int l, int r, T v) { update(l, v); update(r + 1, -v); }
  T get(int i) {
    T ret = T(); for (i++; i > 0; i -= i & -i) ret += BIT[i];
    return ret;
  }
};
