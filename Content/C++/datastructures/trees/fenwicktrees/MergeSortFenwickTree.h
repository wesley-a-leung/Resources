#pragma once
#include <bits/stdc++.h>
#include "../../../search/BinarySearch.h"
using namespace std;

// Merge Sort Fenwick Tree supporting rank and select operations for a subarray
// Indices are 0-indexed and ranges are inclusive with the exception of
//   functions that accept two iterators as a parameter, such as
//   the constructor, which are exclusive
// Template Arguments:
//   T: the type of the element of the array
//   Cmp: the comparator to compare two elements
//     Required Functions:
//       operator (a, b): returns true if and only if a compares less than b
// Constructor Arguments:
//   N: the size of the array
//   f: a generating function that returns the ith element on the ith call
//   st: an iterator pointing to the first element in the array
//   en: an iterator pointing to after the last element in the array
// Functions:
//   rank(l, r, k): returns the number of elements less than k (using the
//     comparator) in the range [l, r]
//   count(l, r, lo, hi) returns the number of elements not less than lo and
//     not greater than hi (using the comparator) in the range [l, r]
//   select(l, r, k): selects the kth element sorted by the comparator if the
//     range [l, r] was sorted
// In practice, has a small constant, slower than WaveletTree for
//   rank and count queries, much slower for select queries,
//   but uses less memory (for integers, coordinate compression can be used
//   otherwise), slightly slower than MergeSortSegmentTree, but uses less
//   memory
// Time Complexity:
//   constructor: O(N log N)
//   rank, count: O((log N)^2)
//   select: O((log N)^3)
// Memory Complexity: O(N log N)
// Tested:
//   https://www.spoj.com/problems/KQUERY/ (rank/count)
//   https://www.spoj.com/problems/KQUERYO/ (rank/count)
//   https://codeforces.com/contest/1284/problem/D (rank/count)
//   https://www.spoj.com/problems/MKTHNUM/ (select)
//   https://judge.yosupo.jp/problem/range_kth_smallest (select)
template <class T, class Cmp = less<T>> struct MergeSortFenwickTree {
  int N; vector<T> sorted; vector<vector<T>> BIT;
  int rank(int r, T k) {
    int ret = 0; for (r++; r > 0; r -= r & -r)
      ret += lower_bound(BIT[r].begin(), BIT[r].end(), k, Cmp())
          - BIT[r].begin();
    return ret;
  }
  int count(int r, T lo, T hi) {
    int ret = 0; for (r++; r > 0; r -= r & -r)
      ret += upper_bound(BIT[r].begin(), BIT[r].end(), hi, Cmp())
          - lower_bound(BIT[r].begin(), BIT[r].end(), lo, Cmp());
    return ret;
  }
  template <class F> MergeSortFenwickTree(int N, F f) : N(N), BIT(N + 1) {
    sorted.reserve(N); vector<int> S(N + 1, 1); for (int i = 1; i <= N; i++) {
      BIT[i].reserve(S[i]); int j = i + (i & -i); if (j <= N) S[j] += S[i];
    }
    for (int i = 1; i <= N; i++) {
      T a = f(); sorted.push_back(a);
      auto it = lower_bound(BIT[i].begin(), BIT[i].end(), a, Cmp());
      BIT[i].insert(it, a); int j = i + (i & -i); if (j <= N) {
        int k = BIT[j].size();
        BIT[j].insert(BIT[j].end(), BIT[i].begin(), BIT[i].end());
        inplace_merge(BIT[j].begin(), BIT[j].begin() + k, BIT[j].end(), Cmp());
      }
    }
    sort(sorted.begin(), sorted.end(), Cmp());
  }
  template <class It> MergeSortFenwickTree(It st, It en)
      : MergeSortFenwickTree(en - st, [&] { return *st++; }) {}
  int rank(int l, int r, T k) { return rank(r, k) - rank(l - 1, k); }
  int count(int l, int r, T lo, T hi) {
    return count(r, lo, hi) - count(l - 1, lo, hi);
  }
  T select(int l, int r, int k) {
    return sorted[bsearch<LAST>(0, N, [&] (int mid) {
      return rank(l, r, sorted[mid]) <= k;
    })];
  }
};
